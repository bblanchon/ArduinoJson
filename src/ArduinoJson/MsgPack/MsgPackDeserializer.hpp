// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/MsgPack/endianess.hpp>
#include <ArduinoJson/MsgPack/ieee754.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(MemoryPool* pool, TReader reader,
                      TStringStorage stringStorage)
      : _pool(pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _foundSomething(false) {}

  template <typename TFilter>
  DeserializationError parse(VariantData& variant, TFilter filter,
                             NestingLimit nestingLimit) {
    DeserializationError::Code err;
    err = parseVariant(&variant, filter, nestingLimit);
    return _foundSomething ? err : DeserializationError::EmptyInput;
  }

 private:
  template <typename TFilter>
  DeserializationError::Code parseVariant(VariantData* variant, TFilter filter,
                                          NestingLimit nestingLimit) {
    DeserializationError::Code err;

    uint8_t code = 0;  // TODO: why do we need to initialize this variable?
    err = readByte(code);
    if (err)
      return err;

    _foundSomething = true;

    bool allowValue = filter.allowValue();

    if (allowValue) {
      // callers pass a null pointer only when value must be ignored
      ARDUINOJSON_ASSERT(variant != 0);
    }

    switch (code) {
      case 0xc0:
        // already null
        return DeserializationError::Ok;

      case 0xc1:
        return DeserializationError::InvalidInput;

      case 0xc2:
        if (allowValue)
          variant->setBoolean(false);
        return DeserializationError::Ok;

      case 0xc3:
        if (allowValue)
          variant->setBoolean(true);
        return DeserializationError::Ok;

      case 0xc4:  // bin 8 (not supported)
        return skipString<uint8_t>();

      case 0xc5:  // bin 16 (not supported)
        return skipString<uint16_t>();

      case 0xc6:  // bin 32 (not supported)
        return skipString<uint32_t>();

      case 0xc7:  // ext 8 (not supported)
        return skipExt<uint8_t>();

      case 0xc8:  // ext 16 (not supported)
        return skipExt<uint16_t>();

      case 0xc9:  // ext 32 (not supported)
        return skipExt<uint32_t>();

      case 0xca:
        if (allowValue)
          return readFloat<float>(variant);
        else
          return skipBytes(4);

      case 0xcb:
        if (allowValue)
          return readDouble<double>(variant);
        else
          return skipBytes(8);

      case 0xcc:
        if (allowValue)
          return readInteger<uint8_t>(variant);
        else
          return skipBytes(1);

      case 0xcd:
        if (allowValue)
          return readInteger<uint16_t>(variant);
        else
          return skipBytes(2);

      case 0xce:
        if (allowValue)
          return readInteger<uint32_t>(variant);
        else
          return skipBytes(4);

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG
        if (allowValue)
          return readInteger<uint64_t>(variant);
        else
          return skipBytes(8);
#else
        return skipBytes(8);  // not supported
#endif

      case 0xd0:
        if (allowValue)
          return readInteger<int8_t>(variant);
        else
          return skipBytes(1);

      case 0xd1:
        if (allowValue)
          return readInteger<int16_t>(variant);
        else
          return skipBytes(2);

      case 0xd2:
        if (allowValue)
          return readInteger<int32_t>(variant);
        else
          return skipBytes(4);

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG
        if (allowValue)
          return readInteger<int64_t>(variant);
        else
          return skipBytes(8);  // not supported
#else
        return skipBytes(8);
#endif

      case 0xd4:  // fixext 1 (not supported)
        return skipBytes(2);

      case 0xd5:  // fixext 2 (not supported)
        return skipBytes(3);

      case 0xd6:  // fixext 4 (not supported)
        return skipBytes(5);

      case 0xd7:  // fixext 8 (not supported)
        return skipBytes(9);

      case 0xd8:  // fixext 16 (not supported)
        return skipBytes(17);

      case 0xd9:
        if (allowValue)
          return readString<uint8_t>(variant);
        else
          return skipString<uint8_t>();

      case 0xda:
        if (allowValue)
          return readString<uint16_t>(variant);
        else
          return skipString<uint16_t>();

      case 0xdb:
        if (allowValue)
          return readString<uint32_t>(variant);
        else
          return skipString<uint32_t>();

      case 0xdc:
        return readArray<uint16_t>(variant, filter, nestingLimit);

      case 0xdd:
        return readArray<uint32_t>(variant, filter, nestingLimit);

      case 0xde:
        return readObject<uint16_t>(variant, filter, nestingLimit);

      case 0xdf:
        return readObject<uint32_t>(variant, filter, nestingLimit);
    }

    switch (code & 0xf0) {
      case 0x80:
        return readObject(variant, code & 0x0F, filter, nestingLimit);

      case 0x90:
        return readArray(variant, code & 0x0F, filter, nestingLimit);
    }

    if ((code & 0xe0) == 0xa0) {
      if (allowValue)
        return readString(variant, code & 0x1f);
      else
        return skipBytes(code & 0x1f);
    }

    if (allowValue)
      variant->setInteger(static_cast<int8_t>(code));

    return DeserializationError::Ok;
  }

  DeserializationError::Code readByte(uint8_t& value) {
    int c = _reader.read();
    if (c < 0)
      return DeserializationError::IncompleteInput;
    value = static_cast<uint8_t>(c);
    return DeserializationError::Ok;
  }

  DeserializationError::Code readBytes(uint8_t* p, size_t n) {
    if (_reader.readBytes(reinterpret_cast<char*>(p), n) == n)
      return DeserializationError::Ok;
    return DeserializationError::IncompleteInput;
  }

  template <typename T>
  DeserializationError::Code readBytes(T& value) {
    return readBytes(reinterpret_cast<uint8_t*>(&value), sizeof(value));
  }

  DeserializationError::Code skipBytes(size_t n) {
    for (; n; --n) {
      if (_reader.read() < 0)
        return DeserializationError::IncompleteInput;
    }
    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError::Code readInteger(T& value) {
    DeserializationError::Code err;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianess(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError::Code readInteger(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readInteger(value);
    if (err)
      return err;

    variant->setInteger(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError::Code>::type
  readFloat(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianess(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, DeserializationError::Code>::type
  readDouble(VariantData* variant) {
    DeserializationError::Code err;
    T value;

    err = readBytes(value);
    if (err)
      return err;

    fixEndianess(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError::Code>::type
  readDouble(VariantData* variant) {
    DeserializationError::Code err;
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t* o = reinterpret_cast<uint8_t*>(&value);

    err = readBytes(i, 8);
    if (err)
      return err;

    doubleToFloat(i, o);
    fixEndianess(value);
    variant->setFloat(value);

    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError::Code readString(VariantData* variant) {
    DeserializationError::Code err;
    T size;

    err = readInteger(size);
    if (err)
      return err;

    return readString(variant, size);
  }

  template <typename T>
  DeserializationError::Code readString() {
    DeserializationError::Code err;
    T size;

    err = readInteger(size);
    if (err)
      return err;

    return readString(size);
  }

  template <typename T>
  DeserializationError::Code skipString() {
    DeserializationError::Code err;
    T size;

    err = readInteger(size);
    if (err)
      return err;

    return skipBytes(size);
  }

  DeserializationError::Code readString(VariantData* variant, size_t n) {
    DeserializationError::Code err;

    err = readString(n);
    if (err)
      return err;

    variant->setString(_stringStorage.save());
    return DeserializationError::Ok;
  }

  DeserializationError::Code readString(size_t n) {
    DeserializationError::Code err;

    _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;

      err = readBytes(c);
      if (err)
        return err;

      _stringStorage.append(static_cast<char>(c));
    }

    if (!_stringStorage.isValid())
      return DeserializationError::NoMemory;

    return DeserializationError::Ok;
  }

  template <typename TSize, typename TFilter>
  DeserializationError::Code readArray(VariantData* variant, TFilter filter,
                                       NestingLimit nestingLimit) {
    DeserializationError::Code err;
    TSize size;

    err = readInteger(size);
    if (err)
      return err;

    return readArray(variant, size, filter, nestingLimit);
  }

  template <typename TFilter>
  DeserializationError::Code readArray(VariantData* variant, size_t n,
                                       TFilter filter,
                                       NestingLimit nestingLimit) {
    DeserializationError::Code err;

    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    bool allowArray = filter.allowArray();

    CollectionData* array = allowArray ? &variant->toArray() : 0;

    TFilter memberFilter = filter[0U];

    for (; n; --n) {
      VariantData* value;

      if (memberFilter.allow()) {
        value = array->addElement(_pool);
        if (!value)
          return DeserializationError::NoMemory;
      } else {
        value = 0;
      }

      err = parseVariant(value, memberFilter, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  template <typename TSize, typename TFilter>
  DeserializationError::Code readObject(VariantData* variant, TFilter filter,
                                        NestingLimit nestingLimit) {
    DeserializationError::Code err;
    TSize size;

    err = readInteger(size);
    if (err)
      return err;

    return readObject(variant, size, filter, nestingLimit);
  }

  template <typename TFilter>
  DeserializationError::Code readObject(VariantData* variant, size_t n,
                                        TFilter filter,
                                        NestingLimit nestingLimit) {
    DeserializationError::Code err;

    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    CollectionData* object = filter.allowObject() ? &variant->toObject() : 0;

    for (; n; --n) {
      err = readKey();
      if (err)
        return err;

      JsonString key = _stringStorage.str();
      TFilter memberFilter = filter[key.c_str()];
      VariantData* member;

      if (memberFilter.allow()) {
        ARDUINOJSON_ASSERT(object);

        // Save key in memory pool.
        // This MUST be done before adding the slot.
        key = _stringStorage.save();

        VariantSlot* slot = object->addSlot(_pool);
        if (!slot)
          return DeserializationError::NoMemory;

        slot->setKey(key);

        member = slot->data();
      } else {
        member = 0;
      }

      err = parseVariant(member, memberFilter, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  DeserializationError::Code readKey() {
    DeserializationError::Code err;
    uint8_t code;

    err = readByte(code);
    if (err)
      return err;

    if ((code & 0xe0) == 0xa0)
      return readString(code & 0x1f);

    switch (code) {
      case 0xd9:
        return readString<uint8_t>();

      case 0xda:
        return readString<uint16_t>();

      case 0xdb:
        return readString<uint32_t>();

      default:
        return DeserializationError::InvalidInput;
    }
  }

  template <typename T>
  DeserializationError::Code skipExt() {
    DeserializationError::Code err;
    T size;

    err = readInteger(size);
    if (err)
      return err;

    return skipBytes(size + 1U);
  }

  MemoryPool* _pool;
  TReader _reader;
  TStringStorage _stringStorage;
  bool _foundSomething;
};

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, const TString& input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, const TString& input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TString>
DeserializationError deserializeMsgPack(JsonDocument& doc, const TString& input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TStream& input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TStream& input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TStream>
DeserializationError deserializeMsgPack(JsonDocument& doc, TStream& input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TChar* input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TChar* input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument& doc, TChar* input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}

// Parses a MessagePack input and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TChar* input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          AllowAllFilter());
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument& doc, TChar* input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}

// Parses a MessagePack input, filters, and puts the result in a JsonDocument.
// https://arduinojson.org/v6/api/msgpack/deserializemsgpack/
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument& doc, TChar* input,
                                        size_t inputSize,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}

}  // namespace ARDUINOJSON_NAMESPACE
