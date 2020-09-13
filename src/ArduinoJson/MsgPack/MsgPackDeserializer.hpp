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
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _error(DeserializationError::Ok),
        _foundSomething(false) {}

  // TODO: add support for filter
  DeserializationError parse(VariantData &variant, AllowAllFilter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, nestingLimit);
    return _foundSomething ? _error : DeserializationError::EmptyInput;
  }

 private:
  bool parseVariant(VariantData &variant, NestingLimit nestingLimit) {
    uint8_t code;
    if (!readByte(code))
      return false;

    _foundSomething = true;

    if ((code & 0x80) == 0) {
      variant.setUnsignedInteger(code);
      return true;
    }

    if ((code & 0xe0) == 0xe0) {
      variant.setSignedInteger(static_cast<int8_t>(code));
      return true;
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
        return true;

      case 0xc2:
        variant.setBoolean(false);
        return true;

      case 0xc3:
        variant.setBoolean(true);
        return true;

      case 0xcc:
        return readInteger<uint8_t>(variant);

      case 0xcd:
        return readInteger<uint16_t>(variant);

      case 0xce:
        return readInteger<uint32_t>(variant);

#if ARDUINOJSON_USE_LONG_LONG
      case 0xcf:
        return readInteger<uint64_t>(variant);
#endif

      case 0xd0:
        return readInteger<int8_t>(variant);

      case 0xd1:
        return readInteger<int16_t>(variant);

      case 0xd2:
        return readInteger<int32_t>(variant);

#if ARDUINOJSON_USE_LONG_LONG
      case 0xd3:
        return readInteger<int64_t>(variant);
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
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);

  bool readByte(uint8_t &value) {
    int c = _reader.read();
    if (c < 0) {
      _error = DeserializationError::IncompleteInput;
      return false;
    }
    value = static_cast<uint8_t>(c);
    return true;
  }

  bool readBytes(uint8_t *p, size_t n) {
    if (_reader.readBytes(reinterpret_cast<char *>(p), n) == n)
      return true;
    _error = DeserializationError::IncompleteInput;
    return false;
  }

  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }

  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    return true;
  }

  template <typename T>
  bool readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value))
      return false;
    variant.setInteger(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, bool>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8))
      return false;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  bool readString(VariantData &variant) {
    T size;
    if (!readInteger(size))
      return false;
    return readString(variant, size);
  }

  template <typename T>
  bool readString(const char *&str) {
    T size;
    if (!readInteger(size))
      return false;
    return readString(str, size);
  }

  bool readString(VariantData &variant, size_t n) {
    const char *s = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
    if (!readString(s, n))
      return false;
    variant.setStringPointer(s, typename TStringStorage::storage_policy());
    return true;
  }

  bool readString(const char *&result, size_t n) {
    _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c))
        return false;
      _stringStorage.append(static_cast<char>(c));
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    result = _stringStorage.save();
    return true;
  }

  template <typename TSize>
  bool readArray(CollectionData &array, NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readArray(array, size, nestingLimit);
  }

  bool readArray(CollectionData &array, size_t n, NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    for (; n; --n) {
      VariantData *value = array.addElement(_pool);
      if (!value) {
        _error = DeserializationError::NoMemory;
        return false;
      }

      if (!parseVariant(*value, nestingLimit.decrement()))
        return false;
    }

    return true;
  }

  template <typename TSize>
  bool readObject(CollectionData &object, NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readObject(object, size, nestingLimit);
  }

  bool readObject(CollectionData &object, size_t n, NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    for (; n; --n) {
      VariantSlot *slot = object.addSlot(_pool);
      if (!slot) {
        _error = DeserializationError::NoMemory;
        return false;
      }

      const char *key = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
      if (!parseKey(key))
        return false;

      slot->setKey(key, typename TStringStorage::storage_policy());

      if (!parseVariant(*slot->data(), nestingLimit.decrement()))
        return false;
    }

    return true;
  }

  bool parseKey(const char *&key) {
    uint8_t code;
    if (!readByte(code))
      return false;

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
        _error = DeserializationError::NotSupported;
        return false;
    }
  }

  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  DeserializationError _error;
  bool _foundSomething;
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
