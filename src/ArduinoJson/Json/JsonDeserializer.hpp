// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Deserialization/deserialize.hpp"
#include "../JsonVariant.hpp"
#include "../Memory/JsonBuffer.hpp"
#include "../Numbers/isFloat.hpp"
#include "../Numbers/isInteger.hpp"
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
  DeserializationError parse(JsonVariantData &variant) {
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

  DeserializationError parseArray(JsonVariantData &variant) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    JsonArrayData *array = new (_buffer) JsonArrayData;
    if (!array) return DeserializationError::NoMemory;
    variant.setArray(*array);

    // Check opening braket
    if (!eat('[')) return DeserializationError::InvalidInput;

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    // Empty array?
    if (eat(']')) return DeserializationError::Ok;

    // Read each value
    for (;;) {
      // Allocate slot in array
      JsonVariantData *value = array->addSlot(_buffer);
      if (!value) return DeserializationError::NoMemory;

      // 1 - Parse value
      _nestingLimit--;
      err = parse(*value);
      _nestingLimit++;
      if (err) return err;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // 3 - More values?
      if (eat(']')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
    }
  }

  DeserializationError parseObject(JsonVariantData &variant) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    JsonObjectData *object = new (_buffer) JsonObjectData;
    if (!object) return DeserializationError::NoMemory;
    variant.setObject(*object);

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

      // Allocate slot in object
      JsonVariantData *value = object->addSlot(_buffer, key);
      if (!value) return DeserializationError::NoMemory;

      // Parse value
      _nestingLimit--;
      err = parse(*value);
      _nestingLimit++;
      if (err) return err;

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

  DeserializationError parseValue(JsonVariantData &variant) {
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

  DeserializationError parseStringValue(JsonVariantData &variant) {
    const char *value;
    DeserializationError err = parseQuotedString(&value);
    if (err) return err;
    variant.setString(value);
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

  DeserializationError parseNumericValue(JsonVariantData &result) {
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
      result.setInteger(parseInteger<JsonInteger>(buffer));
    } else if (isFloat(buffer)) {
      result.setFloat(parseFloat<JsonFloat>(buffer));
    } else if (!strcmp(buffer, "true")) {
      result.setBoolean(true);
    } else if (!strcmp(buffer, "false")) {
      result.setBoolean(false);
    } else if (!strcmp(buffer, "null")) {
      result.setNull();
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
