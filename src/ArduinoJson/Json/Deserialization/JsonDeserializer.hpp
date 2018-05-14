// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../../JsonError.hpp"
#include "../../JsonVariant.hpp"
#include "../../Memory/JsonBuffer.hpp"
#include "../../Reading/Reader.hpp"
#include "../../TypeTraits/IsConst.hpp"
#include "../Encoding.hpp"

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
        _nestingLimit(nestingLimit),
        _loaded(false) {}
  JsonError parse(JsonVariant &variant) {
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    switch (current()) {
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

  char current() {
    if (!_loaded) {
      if (_reader.ended())
        _current = 0;
      else
        _current = _reader.read();
      _loaded = true;
    }
    return _current;
  }

  void move() {
    _loaded = false;
  }

  FORCE_INLINE bool eat(char charToSkip) {
    if (current() != charToSkip) return false;
    move();
    return true;
  }

  JsonError parseArray(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonArray *array = new (_buffer) JsonArray(_buffer);
    if (!array) return JsonError::NoMemory;
    variant = array;

    // Check opening braket
    if (!eat('[')) return JsonError::InvalidInput;

    // Skip spaces
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    // Empty array?
    if (eat(']')) return JsonError::Ok;

    // Read each value
    for (;;) {
      // 1 - Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!array->add(value)) return JsonError::NoMemory;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // 3 - More values?
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

    // Skip spaces
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    // Empty object?
    if (eat('}')) return JsonError::Ok;

    // Read each key value pair
    for (;;) {
      // Parse key
      const char *key;
      err = parseString(&key);
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // Colon
      if (!eat(':')) return JsonError::InvalidInput;

      // Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!object->set(key, value)) return JsonError::NoMemory;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // More keys/values?
      if (eat('}')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;
    }
  }

  JsonError parseValue(JsonVariant &variant) {
    bool hasQuotes = isQuote(current());
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

    char c = current();
    if (c == '\0') return JsonError::IncompleteInput;

    if (isQuote(c)) {  // quotes
      move();
      char stopChar = c;
      for (;;) {
        c = current();
        move();
        if (c == stopChar) break;

        if (c == '\0') return JsonError::IncompleteInput;

        if (c == '\\') {
          c = current();
          if (c == 0) return JsonError::IncompleteInput;
          // replace char
          c = Encoding::unescapeChar(c);
          if (c == '\0') return JsonError::InvalidInput;
          move();
        }

        str.append(c);
      }
    } else if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        str.append(c);
        c = current();
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

  JsonError skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        // end of string
        case '\0':
          return JsonError::IncompleteInput;

        // spaces
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          move();
          continue;

        // comments
        case '/':
          move();  // skip '/'
          switch (current()) {
            // block comment
            case '*': {
              move();  // skip '*'
              bool wasStar = false;
              for (;;) {
                char c = current();
                if (c == '\0') return JsonError::IncompleteInput;
                if (c == '/' && wasStar) {
                  move();
                  break;
                }
                wasStar = c == '*';
                move();
              }
              break;
            }

            // trailing comment
            case '/':
              // no need to skip "//"
              for (;;) {
                move();
                char c = current();
                if (c == '\0') return JsonError::IncompleteInput;
                if (c == '\n') break;
              }
              break;

            // not a comment, just a '/'
            default:
              return JsonError::InvalidInput;
          }
          break;

        default:
          return JsonError::Ok;
      }
    }
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
  char _current;
  bool _loaded;
};

template <typename TJsonBuffer, typename TReader, typename TWriter>
JsonDeserializer<TReader, TWriter> makeJsonDeserializer(TJsonBuffer *buffer,
                                                        TReader reader,
                                                        TWriter writer,
                                                        uint8_t nestingLimit) {
  return JsonDeserializer<TReader, TWriter>(buffer, reader, writer,
                                            nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
