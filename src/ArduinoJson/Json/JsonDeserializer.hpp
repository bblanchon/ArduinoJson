// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Deserialization/deserialize.hpp"
#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../Polyfills/type_traits.hpp"
#include "./EscapeSequence.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TReader, typename TStringStorage>
class JsonDeserializer {
 public:
  JsonDeserializer(JsonBuffer *buffer, TReader reader,
                   TStringStorage stringStorage, uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _stringStorage(stringStorage),
        _nestingLimit(nestingLimit),
        _loaded(false) {}
  DeserializationError parse(JsonVariant &variant) {
    DeserializationError err = skipSpacesAndComments();
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

  DeserializationError parseArray(JsonVariant &variant) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    JsonArray array(_buffer);
    if (array.isNull()) return DeserializationError::NoMemory;
    variant = array;

    // Check opening braket
    if (!eat('[')) return DeserializationError::InvalidInput;

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    // Empty array?
    if (eat(']')) return DeserializationError::Ok;

    // Read each value
    for (;;) {
      // 1 - Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!array.add(value)) return DeserializationError::NoMemory;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // 3 - More values?
      if (eat(']')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
    }
  }

  DeserializationError parseObject(JsonVariant &variant) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    JsonObject object(_buffer);
    if (object.isNull()) return DeserializationError::NoMemory;
    variant = object;

    // Check opening brace
    if (!eat('{')) return DeserializationError::InvalidInput;

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    // Empty object?
    if (eat('}')) return DeserializationError::Ok;

    // Read each key value pair
    for (;;) {
      // Parse key
      const char *key;
      err = parseKey(&key);
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;  // Colon
      if (!eat(':')) return DeserializationError::InvalidInput;

      // Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!object.set(key, value)) return DeserializationError::NoMemory;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // More keys/values?
      if (eat('}')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;
    }
  }

  DeserializationError parseValue(JsonVariant &variant) {
    if (isQuote(current())) {
      return parseStringValue(variant);
    } else {
      return parseNumericValue(variant);
    }
  }

  DeserializationError parseKey(const char **key) {
    if (isQuote(current())) {
      return parseQuotedString(key);
    } else {
      return parseNonQuotedString(key);
    }
  }

  DeserializationError parseStringValue(JsonVariant &variant) {
    const char *value;
    DeserializationError err = parseQuotedString(&value);
    if (err) return err;
    variant = value;
    return DeserializationError::Ok;
  }

  DeserializationError parseQuotedString(const char **result) {
    typename remove_reference<TStringStorage>::type::String str =
        _stringStorage.startString();

    char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar) break;

      if (c == '\0') return DeserializationError::IncompleteInput;

      if (c == '\\') {
        c = current();
        if (c == '\0') return DeserializationError::IncompleteInput;
        if (c == 'u') return DeserializationError::NotSupported;
        // replace char
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0') return DeserializationError::InvalidInput;
        move();
      }

      str.append(c);
    }

    *result = str.c_str();
    if (*result == NULL) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }

  DeserializationError parseNonQuotedString(const char **result) {
    typename remove_reference<TStringStorage>::type::String str =
        _stringStorage.startString();

    char c = current();
    if (c == '\0') return DeserializationError::IncompleteInput;

    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        str.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      return DeserializationError::InvalidInput;
    }

    *result = str.c_str();
    if (*result == NULL) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }

  DeserializationError parseNumericValue(JsonVariant &result) {
    char buffer[64];
    uint8_t n = 0;

    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      buffer[n++] = c;
      c = current();
    }
    buffer[n] = 0;

    if (isInteger(buffer)) {
      result = parseInteger<JsonInteger>(buffer);
    } else if (isFloat(buffer)) {
      result = parseFloat<JsonFloat>(buffer);
    } else if (!strcmp(buffer, "true")) {
      result = true;
    } else if (!strcmp(buffer, "false")) {
      result = false;
    } else if (!strcmp(buffer, "null")) {
      result = static_cast<const char *>(0);
    } else {
      return DeserializationError::InvalidInput;
    }
    return DeserializationError::Ok;
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

  DeserializationError skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        // end of string
        case '\0':
          return DeserializationError::IncompleteInput;

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
                if (c == '\0') return DeserializationError::IncompleteInput;
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
                if (c == '\0') return DeserializationError::IncompleteInput;
                if (c == '\n') break;
              }
              break;

            // not a comment, just a '/'
            default:
              return DeserializationError::InvalidInput;
          }
          break;

        default:
          return DeserializationError::Ok;
      }
    }
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TStringStorage _stringStorage;
  uint8_t _nestingLimit;
  char _current;
  bool _loaded;
};  // namespace Internals
}  // namespace Internals

template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, const TInput &input) {
  using namespace Internals;
  return deserialize<JsonDeserializer>(doc, input);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput *input) {
  using namespace Internals;
  return deserialize<JsonDeserializer>(doc, input);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput *input,
                                     size_t inputSize) {
  using namespace Internals;
  return deserialize<JsonDeserializer>(doc, input, inputSize);
}

template <typename TDocument, typename TInput>
DeserializationError deserializeJson(TDocument &doc, TInput &input) {
  using namespace Internals;
  return deserialize<JsonDeserializer>(doc, input);
}
}  // namespace ArduinoJson
