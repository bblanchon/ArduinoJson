// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../../JsonError.hpp"
#include "../../JsonVariant.hpp"
#include "../../Memory/JsonBuffer.hpp"
#include "../../Strings/StringWriter.hpp"
#include "../../TypeTraits/IsConst.hpp"
#include "../Encoding.hpp"
#include "./Comments.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TReader, typename TWriter>
class JsonDeserializer {
 public:
  JsonDeserializer(JsonBuffer *buffer, TReader reader, TWriter writer,
                   uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _writer(writer),
        _nestingLimit(nestingLimit) {}
  JsonError parse(JsonVariant &variant) {
    skipSpacesAndComments(_reader);

    switch (_reader.current()) {
      case '[':
        return parseArray(variant);

      case '{':
        return parseObject(variant);

      default:
        return parseValue(variant);
    }
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

  static bool eat(TReader &reader, char charToSkip) {
    skipSpacesAndComments(reader);
    if (reader.current() != charToSkip) return false;
    reader.move();
    return true;
  }

  FORCE_INLINE bool eat(char charToSkip) {
    return eat(_reader, charToSkip);
  }

  JsonError parseArray(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonArray *array = new (_buffer) JsonArray(_buffer);
    if (!array) return JsonError::NoMemory;
    variant = array;

    // Check opening braket
    if (!eat('[')) return JsonError::InvalidInput;
    if (eat(']')) return JsonError::Ok;

    // Read each value
    for (;;) {
      // 1 - Parse value
      JsonVariant value;
      _nestingLimit--;
      JsonError error = parse(value);
      _nestingLimit++;
      if (error != JsonError::Ok) return error;
      if (!array->add(value)) return JsonError::NoMemory;

      // 2 - More values?
      if (eat(']')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;
    }
  }

  JsonError parseObject(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonObject *object = new (_buffer) JsonObject(_buffer);
    if (!object) return JsonError::NoMemory;
    variant = object;

    // Check opening brace
    if (!eat('{')) return JsonError::InvalidInput;
    if (eat('}')) return JsonError::Ok;

    // Read each key value pair
    for (;;) {
      // 1 - Parse key
      const char *key;
      JsonError error = parseString(&key);
      if (error) return error;
      if (!eat(':')) return JsonError::InvalidInput;

      // 2 - Parse value
      JsonVariant value;
      _nestingLimit--;
      error = parse(value);
      _nestingLimit++;
      if (error != JsonError::Ok) return error;
      if (!object->set(key, value)) return JsonError::NoMemory;

      // 3 - More keys/values?
      if (eat('}')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;
    }
  }
  JsonError parseValue(JsonVariant &variant) {
    bool hasQuotes = isQuote(_reader.current());
    const char *value;
    JsonError error = parseString(&value);
    if (error) return error;
    if (hasQuotes) {
      variant = value;
    } else {
      variant = RawJson(value);
    }
    return JsonError::Ok;
  }

  JsonError parseString(const char **result) {
    typename RemoveReference<TWriter>::type::String str = _writer.startString();

    skipSpacesAndComments(_reader);
    char c = _reader.current();

    if (isQuote(c)) {  // quotes
      _reader.move();
      char stopChar = c;
      for (;;) {
        c = _reader.current();
        if (c == '\0') break;
        _reader.move();

        if (c == stopChar) break;

        if (c == '\\') {
          // replace char
          c = Encoding::unescapeChar(_reader.current());
          if (c == '\0') return JsonError::InvalidInput;
          _reader.move();
        }

        str.append(c);
      }
    } else if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        _reader.move();
        str.append(c);
        c = _reader.current();
      } while (canBeInNonQuotedString(c));
    } else {
      return JsonError::InvalidInput;
    }

    *result = str.c_str();
    if (*result == NULL) return JsonError::NoMemory;
    return JsonError::Ok;
  }

  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }

  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TString, typename Enable = void>
struct JsonParserBuilder {
  typedef typename StringTraits<TString>::Reader InputReader;
  typedef JsonDeserializer<InputReader, TJsonBuffer &> TParser;

  static TParser makeParser(TJsonBuffer *buffer, TString &json,
                            uint8_t nestingLimit) {
    return TParser(buffer, InputReader(json), *buffer, nestingLimit);
  }
};

template <typename TJsonBuffer, typename TChar>
struct JsonParserBuilder<TJsonBuffer, TChar *,
                         typename EnableIf<!IsConst<TChar>::value>::type> {
  typedef typename StringTraits<TChar *>::Reader TReader;
  typedef StringWriter<TChar> TWriter;
  typedef JsonDeserializer<TReader, TWriter> TParser;

  static TParser makeParser(TJsonBuffer *buffer, TChar *json,
                            uint8_t nestingLimit) {
    return TParser(buffer, TReader(json), TWriter(json), nestingLimit);
  }
};

template <typename TJsonBuffer, typename TString>
inline typename JsonParserBuilder<TJsonBuffer, TString>::TParser makeParser(
    TJsonBuffer *buffer, TString &json, uint8_t nestingLimit) {
  return JsonParserBuilder<TJsonBuffer, TString>::makeParser(buffer, json,
                                                             nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
