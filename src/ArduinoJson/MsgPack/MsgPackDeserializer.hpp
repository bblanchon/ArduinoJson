// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Deserialization/deserialize.hpp"
#include "../JsonVariant.hpp"
#include "../Memory/MemoryPool.hpp"
#include "../Polyfills/type_traits.hpp"
#include "./endianess.hpp"
#include "./ieee754.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(MemoryPool &memoryPool, TReader reader,
                      TStringStorage stringStorage, uint8_t nestingLimit)
      : _memoryPool(&memoryPool),
        _reader(reader),
        _stringStorage(stringStorage),
        _nestingLimit(nestingLimit) {}

  DeserializationError parse(JsonVariant variant) {
    uint8_t code;
    if (!readByte(code)) return DeserializationError::IncompleteInput;

    if ((code & 0x80) == 0) {
      variant.set(code);
      return DeserializationError::Ok;
    }

    if ((code & 0xe0) == 0xe0) {
      variant.set(static_cast<int8_t>(code));
      return DeserializationError::Ok;
    }

    if ((code & 0xe0) == 0xa0) {
      return readString(variant, code & 0x1f);
    }

    if ((code & 0xf0) == 0x90) return readArray(variant, code & 0x0F);

    if ((code & 0xf0) == 0x80) return readObject(variant, code & 0x0F);

    switch (code) {
      case 0xc0:
        // already null
        return DeserializationError::Ok;

      case 0xc2:
        variant.set(false);
        return DeserializationError::Ok;

      case 0xc3:
        variant.set(true);
        return DeserializationError::Ok;

      case 0xcc:
        return readInteger<uint8_t>(variant);

      case 0xcd:
        return readInteger<uint16_t>(variant);

      case 0xce:
        return readInteger<uint32_t>(variant);

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        return readInteger<uint64_t>(variant);
#else
        readInteger<uint32_t>();
        return readInteger<uint32_t>(variant);
#endif

      case 0xd0:
        return readInteger<int8_t>(variant);

      case 0xd1:
        return readInteger<int16_t>(variant);

      case 0xd2:
        return readInteger<int32_t>(variant);

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        return readInteger<int64_t>(variant);
#else
        if (!skip(4)) return DeserializationError::IncompleteInput;
        return readInteger<int32_t>(variant);
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
        return readArray<uint16_t>(variant);

      case 0xdd:
        return readArray<uint32_t>(variant);

      case 0xde:
        return readObject<uint16_t>(variant);

      case 0xdf:
        return readObject<uint32_t>(variant);

      default:
        return DeserializationError::NotSupported;
    }
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);

  bool skip(uint8_t n) {
    while (n--) {
      if (_reader.ended()) return false;
      _reader.read();
    }
    return true;
  }

  bool readByte(uint8_t &value) {
    if (_reader.ended()) return false;
    value = static_cast<uint8_t>(_reader.read());
    return true;
  }

  bool readBytes(uint8_t *p, size_t n) {
    for (size_t i = 0; i < n; i++) {
      if (!readByte(p[i])) return false;
    }
    return true;
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
    if (!readBytes(value)) return false;
    fixEndianess(value);
    return true;
  }

  template <typename T>
  DeserializationError readInteger(JsonVariant variant) {
    T value;
    if (!readInteger(value)) return DeserializationError::IncompleteInput;
    variant.set(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readFloat(
      JsonVariant variant) {
    T value;
    if (!readBytes(value)) return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.set(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, DeserializationError>::type readDouble(
      JsonVariant variant) {
    T value;
    if (!readBytes(value)) return DeserializationError::IncompleteInput;
    fixEndianess(value);
    variant.set(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, DeserializationError>::type readDouble(
      JsonVariant variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8)) return DeserializationError::IncompleteInput;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.set(value);
    return DeserializationError::Ok;
  }

  template <typename T>
  DeserializationError readString(JsonVariant variant) {
    T size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readString(variant, size);
  }

  DeserializationError readString(JsonVariant variant, size_t n) {
    typename remove_reference<TStringStorage>::type::String str =
        _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c)) return DeserializationError::IncompleteInput;
      str.append(static_cast<char>(c));
    }
    const char *s = str.c_str();
    if (s == NULL) return DeserializationError::NoMemory;
    variant.set(s);
    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readArray(JsonVariant variant) {
    TSize size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readArray(variant, size);
  }

  DeserializationError readArray(JsonVariant variant, size_t n) {
    JsonArray array = variant.to<JsonArray>();
    if (array.isNull()) return DeserializationError::NoMemory;
    return readArray(array, n);
  }

  DeserializationError readArray(JsonArray array, size_t n) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      JsonVariant value = array.add();
      if (value.isInvalid()) return DeserializationError::NoMemory;

      DeserializationError err = parse(value);
      if (err) return err;
    }
    ++_nestingLimit;
    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readObject(JsonVariant variant) {
    TSize size;
    if (!readInteger(size)) return DeserializationError::IncompleteInput;
    return readObject(variant, size);
  }

  DeserializationError readObject(JsonVariant variant, size_t n) {
    JsonObject object = variant.to<JsonObject>();
    if (object.isNull()) return DeserializationError::NoMemory;

    return readObject(object, n);
  }

  DeserializationError readObject(JsonObject object, size_t n) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      JsonVariantData keyData;
      JsonVariant key(_memoryPool, &keyData);
      DeserializationError err = parse(key);
      if (err) return err;
      if (!keyData.isString()) return DeserializationError::NotSupported;

      JsonVariant value = object.set(keyData.asString());
      if (value.isInvalid()) return DeserializationError::NoMemory;

      err = parse(value);
      if (err) return err;
    }
    ++_nestingLimit;
    return DeserializationError::Ok;
  }

  MemoryPool *_memoryPool;
  TReader _reader;
  TStringStorage _stringStorage;
  uint8_t _nestingLimit;
};
}  // namespace Internals

template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, const TInput &input) {
  using namespace Internals;
  return deserialize<MsgPackDeserializer>(doc, input);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput *input) {
  using namespace Internals;
  return deserialize<MsgPackDeserializer>(doc, input);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput *input,
                                        size_t inputSize) {
  using namespace Internals;
  return deserialize<MsgPackDeserializer>(doc, input, inputSize);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeMsgPack(TDocument &doc, TInput &input) {
  using namespace Internals;
  return deserialize<MsgPackDeserializer>(doc, input);
}
}  // namespace ArduinoJson
