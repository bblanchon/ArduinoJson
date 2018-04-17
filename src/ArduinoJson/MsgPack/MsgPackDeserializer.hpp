// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../Strings/StringWriter.hpp"
#include "../TypeTraits/IsConst.hpp"
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
    uint8_t c = readOne();

    if ((c & 0x80) == 0) {
      variant = c;
      return MsgPackError::Ok;
    }

    if ((c & 0xe0) == 0xe0) {
      variant = static_cast<int8_t>(c);
      return MsgPackError::Ok;
    }

    if ((c & 0xe0) == 0xa0) {
      return readString(variant, c & 0x1f);
    }

    if ((c & 0xf0) == 0x90) return readArray(variant, c & 0x0F);

    if ((c & 0xf0) == 0x80) return readObject(variant, c & 0x0F);

    switch (c) {
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
        variant = readInteger<uint8_t>();
        return MsgPackError::Ok;

      case 0xcd:
        variant = readInteger<uint16_t>();
        return MsgPackError::Ok;

      case 0xce:
        variant = readInteger<uint32_t>();
        return MsgPackError::Ok;

      case 0xcf:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        variant = readInteger<uint64_t>();
#else
        readInteger<uint32_t>();
        variant = readInteger<uint32_t>();
#endif
        return MsgPackError::Ok;

      case 0xd0:
        variant = readInteger<int8_t>();
        return MsgPackError::Ok;

      case 0xd1:
        variant = readInteger<int16_t>();
        return MsgPackError::Ok;

      case 0xd2:
        variant = readInteger<int32_t>();
        return MsgPackError::Ok;

      case 0xd3:
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
        variant = readInteger<int64_t>();
#else
        readInteger<int32_t>();
        variant = readInteger<int32_t>();
#endif
        return MsgPackError::Ok;

      case 0xca:
        variant = readFloat<float>();
        return MsgPackError::Ok;

      case 0xcb:
        variant = readDouble<double>();
        return MsgPackError::Ok;

      case 0xd9: {
        uint8_t n = readInteger<uint8_t>();
        return readString(variant, n);
      }

      case 0xda: {
        uint16_t n = readInteger<uint16_t>();
        return readString(variant, n);
      }

      case 0xdb: {
        uint32_t n = readInteger<uint32_t>();
        return readString(variant, n);
      }

      case 0xdc:
        return readArray(variant, readInteger<uint16_t>());

      case 0xdd:
        return readArray(variant, readInteger<uint32_t>());

      case 0xde:
        return readObject(variant, readInteger<uint16_t>());

      case 0xdf:
        return readObject(variant, readInteger<uint32_t>());

      default:
        return MsgPackError::NotSupported;
    }
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);

  uint8_t readOne() {
    char c = _reader.current();
    _reader.move();
    return static_cast<uint8_t>(c);
  }

  void read(uint8_t *p, size_t n) {
    for (size_t i = 0; i < n; i++) p[i] = readOne();
  }

  template <typename T>
  void read(T &value) {
    read(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }

  template <typename T>
  T readInteger() {
    T value;
    read(value);
    fixEndianess(value);
    return value;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, T>::type readFloat() {
    T value;
    read(value);
    fixEndianess(value);
    return value;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 8, T>::type readDouble() {
    T value;
    read(value);
    fixEndianess(value);
    return value;
  }

  template <typename T>
  typename EnableIf<sizeof(T) == 4, T>::type readDouble() {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    read(i, 8);
    doubleToFloat(i, o);
    fixEndianess(value);
    return value;
  }

  MsgPackError readString(JsonVariant &variant, size_t n) {
    typename RemoveReference<TWriter>::type::String str = _writer.startString();
    for (; n; --n) str.append(static_cast<char>(readOne()));
    const char *s = str.c_str();
    if (s == NULL) return MsgPackError::NoMemory;
    variant = s;
    return MsgPackError::Ok;
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

template <typename TJsonBuffer, typename TString, typename Enable = void>
struct MsgPackDeserializerBuilder {
  typedef typename StringTraits<TString>::Reader InputReader;
  typedef MsgPackDeserializer<InputReader, TJsonBuffer &> TParser;

  static TParser makeMsgPackDeserializer(TJsonBuffer *buffer, TString &json,
                                         uint8_t nestingLimit) {
    return TParser(buffer, InputReader(json), *buffer, nestingLimit);
  }
};

template <typename TJsonBuffer, typename TChar>
struct MsgPackDeserializerBuilder<
    TJsonBuffer, TChar *, typename EnableIf<!IsConst<TChar>::value>::type> {
  typedef typename StringTraits<TChar *>::Reader TReader;
  typedef StringWriter<TChar> TWriter;
  typedef MsgPackDeserializer<TReader, TWriter> TParser;

  static TParser makeMsgPackDeserializer(TJsonBuffer *buffer, TChar *json,
                                         uint8_t nestingLimit) {
    return TParser(buffer, TReader(json), TWriter(json), nestingLimit);
  }
};

template <typename TJsonBuffer, typename TString>
inline typename MsgPackDeserializerBuilder<TJsonBuffer, TString>::TParser
makeMsgPackDeserializer(TJsonBuffer *buffer, TString &json,
                        uint8_t nestingLimit) {
  return MsgPackDeserializerBuilder<
      TJsonBuffer, TString>::makeMsgPackDeserializer(buffer, json,
                                                     nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
