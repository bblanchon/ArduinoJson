// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../Reading/Reader.hpp"
#include "../TypeTraits/IsConst.hpp"
#include "../Writing/Writer.hpp"
#include "./MsgPackError.hpp"
#include "./endianess.hpp"
#include "./ieee754.hpp"

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
template <typename TReader, typename TWriter>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(JsonBuffer *buffer, TReader reader, TWriter writer,
                      uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _writer(writer),
        _nestingLimit(nestingLimit) {}

  MsgPackError parse(JsonVariant &variant) {
    uint8_t code;
    if (!readByte(code)) return MsgPackError::IncompleteInput;

    if ((code & 0x80) == 0) {
      variant = code;
      return MsgPackError::Ok;
    }

    if ((code & 0xe0) == 0xe0) {
      variant = static_cast<int8_t>(code);
      return MsgPackError::Ok;
    }

    if ((code & 0xe0) == 0xa0) {
      return readString(variant, code & 0x1f);
    }

    if ((code & 0xf0) == 0x90) return readArray(variant, code & 0x0F);

    if ((code & 0xf0) == 0x80) return readObject(variant, code & 0x0F);

    switch (code) {
      case 0xc0:
        variant = static_cast<char *>(0);
        return MsgPackError::Ok;

      case 0xc2:
        variant = false;
        return MsgPackError::Ok;

      case 0xc3:
        variant = true;
        return MsgPackError::Ok;

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
        if (!skip(4)) return MsgPackError::IncompleteInput;
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
        return MsgPackError::NotSupported;
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
  MsgPackError readInteger(JsonVariant &variant) {
    T value;
    if (!readInteger(value)) return MsgPackError::IncompleteInput;
    variant = value;
    return MsgPackError::Ok;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, MsgPackError>::type readFloat(
      JsonVariant &variant) {
    T value;
    if (!readBytes(value)) return MsgPackError::IncompleteInput;
    fixEndianess(value);
    variant = value;
    return MsgPackError::Ok;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 8, MsgPackError>::type readDouble(
      JsonVariant &variant) {
    T value;
    if (!readBytes(value)) return MsgPackError::IncompleteInput;
    fixEndianess(value);
    variant = value;
    return MsgPackError::Ok;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, MsgPackError>::type readDouble(
      JsonVariant &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8)) return MsgPackError::IncompleteInput;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant = value;
    return MsgPackError::Ok;
  }

  template <typename T>
  MsgPackError readString(JsonVariant &variant) {
    T size;
    if (!readInteger(size)) return MsgPackError::IncompleteInput;
    return readString(variant, size);
  }

  MsgPackError readString(JsonVariant &variant, size_t n) {
    typename RemoveReference<TWriter>::type::String str = _writer.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c)) return MsgPackError::IncompleteInput;
      str.append(static_cast<char>(c));
    }
    const char *s = str.c_str();
    if (s == NULL) return MsgPackError::NoMemory;
    variant = s;
    return MsgPackError::Ok;
  }

  template <typename TSize>
  MsgPackError readArray(JsonVariant &variant) {
    TSize size;
    if (!readInteger(size)) return MsgPackError::IncompleteInput;
    return readArray(variant, size);
  }

  MsgPackError readArray(JsonVariant &variant, size_t n) {
    JsonArray *array = new (_buffer) JsonArray(_buffer);
    if (!array) return MsgPackError::NoMemory;
    variant = array;
    return readArray(*array, n);
  }

  MsgPackError readArray(JsonArray &array, size_t n) {
    if (_nestingLimit == 0) return MsgPackError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      JsonVariant variant;
      MsgPackError err = parse(variant);
      if (err) return err;
      if (!array.add(variant)) return MsgPackError::NoMemory;
    }
    ++_nestingLimit;
    return MsgPackError::Ok;
  }

  template <typename TSize>
  MsgPackError readObject(JsonVariant &variant) {
    TSize size;
    if (!readInteger(size)) return MsgPackError::IncompleteInput;
    return readObject(variant, size);
  }

  MsgPackError readObject(JsonVariant &variant, size_t n) {
    JsonObject *object = new (_buffer) JsonObject(_buffer);
    if (!object) return MsgPackError::NoMemory;
    variant = object;
    return readObject(*object, n);
  }

  MsgPackError readObject(JsonObject &object, size_t n) {
    if (_nestingLimit == 0) return MsgPackError::TooDeep;
    --_nestingLimit;
    for (; n; --n) {
      MsgPackError err;
      JsonVariant variant;
      err = parse(variant);
      if (err) return err;
      const char *key = variant.as<char *>();
      if (!key) return MsgPackError::NotSupported;
      err = parse(variant);
      if (err) return err;
      if (!object.set(key, variant)) return MsgPackError::NoMemory;
    }
    ++_nestingLimit;
    return MsgPackError::Ok;
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TReader, typename TWriter>
MsgPackDeserializer<TReader, TWriter> makeMsgPackDeserializer(
    TJsonBuffer *buffer, TReader reader, TWriter writer, uint8_t nestingLimit) {
  return MsgPackDeserializer<TReader, TWriter>(buffer, reader, writer,
                                               nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
