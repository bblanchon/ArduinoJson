// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
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
  MsgPackDeserializer(MemoryPool &pool, TReader reader,
                      TStringStorage stringStorage)
      : _pool(&pool), _reader(reader), _stringStorage(stringStorage) {}

  // TODO: add support for filter
  DeserializationError parse(VariantData &variant, AllowAllFilter,
                             NestingLimit nestingLimit) {
    return parse(variant, nestingLimit);
  }

  DeserializationError parse(VariantData &variant, NestingLimit nestingLimit) {
    uint8_t code;
    if (!readByte(code))
      return DeserializationError::IncompleteInput;

    if ((code & 0x80) == 0) {
      variant.setUnsignedInteger(code);
      return DeserializationError::Ok;
    }

    if ((code & 0xe0) == 0xe0) {
      // TODO: add setNegativeInteger()
      variant.setSignedInteger(static_cast<int8_t>(code));
      return DeserializationError::Ok;
    }

    if ((code & 0xe0) == 0xa0) {
      return readString(variant, code & 0x1f);
    }

    if ((code & 0xf0) == 0x90) {
      return readArray(variant.toArray(), code & 0x0F, nestingLimit);
    }

    if ((code & 0xf0) == 0x80) {
      return readObject(variant.toObject(), code & 0x0F, nestingLimit);
    }

    switch (code) {
      case 0xc0:
        // already null
        return DeserializationError::Ok;

      case 0xc2:
        variant.setBoolean(false);
        return DeserializationError::Ok;

      case 0xc3:
        variant.setBoolean(true);
        return DeserializationError::Ok;

      case 0xcc:
        return readInteger<uint8_t>(variant);

      case 0xcd:
        return readInteger<uint16_t>(variant);

      case 0xce:
        return readInteger<uint32_t>(variant);

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG
        return readInteger<uint64_t>(variant);
#else
        return DeserializationError::NotSupported;
#endif

      case 0xd0:
        return readInteger<int8_t>(variant);

      case 0xd1:
        return readInteger<int16_t>(variant);

      case 0xd2:
        return readInteger<int32_t>(variant);

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG
        return readInteger<int64_t>(variant);
#else
        return DeserializationError::NotSupported;
#endif

      case 0xca:
        return readFloat<float>(variant);

      case 0xcb:
        return readDouble<double>(variant);

      case 0xd9:
        return readString<uint8_t>(variant);

      case 0xda:
        return readString<uint16_t>(variant);

      case 0xdb:
        return readString<uint32_t>(variant);

      case 0xdc:
        return readArray<uint16_t>(variant.toArray(), nestingLimit);

      case 0xdd:
        return readArray<uint32_t>(variant.toArray(), nestingLimit);

      case 0xde:
        return readObject<uint16_t>(variant.toObject(), nestingLimit);

      case 0xdf:
        return readObject<uint32_t>(variant.toObject(), nestingLimit);

      default:
        return DeserializationError::NotSupported;
    }
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);

  bool readByte(uint8_t &value) {
    int c = _reader.read();
    if (c < 0)
      return false;
    value = static_cast<uint8_t>(c);
    return true;
  }

  bool readBytes(uint8_t *p, size_t n) {
    return _reader.readBytes(reinterpret_cast<char *>(p), n) == n;
  }

  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }

  template <typename T>
  T readInteger() {
    T value;
    readBytes(value);
    fixEndianess(value);
    return value;
  }

  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    return true;
  }

  template <typename T>
  DeserializationError readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value))
      return DeserializationError::IncompleteInput;
    variant.setInteger(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, DeserializationError>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8))
      return DeserializationError::IncompleteInput;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError readString(VariantData &variant) {
    T size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readString(variant, size);
  }

  template <typename T>
  DeserializationError readString(const char *&str) {
    T size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readString(str, size);
  }

  DeserializationError readString(VariantData &variant, size_t n) {
    const char *s = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
    DeserializationError err = readString(s, n);
    if (!err)
      variant.setOwnedString(make_not_null(s));
    return err;
  }

  DeserializationError readString(const char *&result, size_t n) {
    _stringStorage.startString(_pool);
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c))
        return DeserializationError::IncompleteInput;
      _stringStorage.append(static_cast<char>(c));
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid())
      return DeserializationError::NoMemory;
    _stringStorage.commit(_pool);
    result = _stringStorage.c_str();
    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readArray(CollectionData &array,
                                 NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readArray(array, size, nestingLimit);
  }

  DeserializationError readArray(CollectionData &array, size_t n,
                                 NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    for (; n; --n) {
      VariantData *value = array.addElement(_pool);
      if (!value)
        return DeserializationError::NoMemory;

      DeserializationError err = parse(*value, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readObject(CollectionData &object,
                                  NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readObject(object, size, nestingLimit);
  }

  DeserializationError readObject(CollectionData &object, size_t n,
                                  NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    for (; n; --n) {
      VariantSlot *slot = object.addSlot(_pool);
      if (!slot)
        return DeserializationError::NoMemory;

      const char *key = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
      DeserializationError err = parseKey(key);
      if (err)
        return err;
      slot->setOwnedKey(make_not_null(key));

      err = parse(*slot->data(), nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  DeserializationError parseKey(const char *&key) {
    uint8_t code;
    if (!readByte(code))
      return DeserializationError::IncompleteInput;

    if ((code & 0xe0) == 0xa0)
      return readString(key, code & 0x1f);

    switch (code) {
      case 0xd9:
        return readString<uint8_t>(key);

      case 0xda:
        return readString<uint16_t>(key);

      case 0xdb:
        return readString<uint32_t>(key);

      default:
        return DeserializationError::NotSupported;
    }
  }

  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
};

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          AllowAllFilter());
}

template <typename TInput>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
}  // namespace ARDUINOJSON_NAMESPACE
