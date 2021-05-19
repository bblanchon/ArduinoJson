// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Json/EscapeSequence.hpp>
#include <ArduinoJson/Json/Latch.hpp>
#include <ArduinoJson/Json/Utf16.hpp>
#include <ArduinoJson/Json/Utf8.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class JsonDeserializer {
 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage)
      : _stringStorage(stringStorage),
        _foundSomething(false),
        _latch(reader),
        _pool(&pool),
        _error(DeserializationError::Ok) {}

  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, filter, nestingLimit);

    if (!_error && _latch.last() != 0 && !variant.isEnclosed()) {
      // We don't detect trailing characters earlier, so we need to check now
      return DeserializationError::InvalidInput;
    }

    return _error;
  }

 private:
  char current() {
    return _latch.current();
  }

  void move() {
    _latch.clear();
  }

  bool eat(char charToSkip) {
    if (current() != charToSkip)
      return false;
    move();
    return true;
  }

  template <typename TFilter>
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;

    switch (current()) {
      case '[':
        if (filter.allowArray())
          return parseArray(variant.toArray(), filter, nestingLimit);
        else
          return skipArray(nestingLimit);

      case '{':
        if (filter.allowObject())
          return parseObject(variant.toObject(), filter, nestingLimit);
        else
          return skipObject(nestingLimit);

      case '\"':
      case '\'':
        if (filter.allowValue())
          return parseStringValue(variant);
        else
          return skipString();

      default:
        if (filter.allowValue())
          return parseNumericValue(variant);
        else
          return skipNumericValue();
    }
  }

  bool skipVariant(NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;

    switch (current()) {
      case '[':
        return skipArray(nestingLimit);

      case '{':
        return skipObject(nestingLimit);

      case '\"':
      case '\'':
        return skipString();

      default:
        return skipNumericValue();
    }
  }

  template <typename TFilter>
  bool parseArray(CollectionData &array, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Skip spaces
    if (!skipSpacesAndComments())
      return false;

    // Empty array?
    if (eat(']'))
      return true;

    TFilter memberFilter = filter[0UL];

    // Read each value
    for (;;) {
      if (memberFilter.allow()) {
        // Allocate slot in array
        VariantData *value = array.addElement(_pool);
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }

        // 1 - Parse value
        if (!parseVariant(*value, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }

      // 2 - Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // 3 - More values?
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  bool skipArray(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Read each value
    for (;;) {
      // 1 - Skip value
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // 2 - Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // 3 - More values?
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  template <typename TFilter>
  bool parseObject(CollectionData &object, TFilter filter,
                   NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
    if (!skipSpacesAndComments())
      return false;

    // Empty object?
    if (eat('}'))
      return true;

    // Read each key value pair
    for (;;) {
      // Parse key
      if (!parseKey())
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // Colon
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      const char *key = _stringStorage.c_str();

      TFilter memberFilter = filter[key];

      if (memberFilter.allow()) {
        VariantData *variant = object.getMember(adaptString(key));
        if (!variant) {
          // Save key in memory pool.
          // This MUST be done before adding the slot.
          key = _stringStorage.save();

          // Allocate slot in object
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot) {
            _error = DeserializationError::NoMemory;
            return false;
          }

          slot->setKey(key, typename TStringStorage::storage_policy());

          variant = slot->data();
        }

        // Parse value
        if (!parseVariant(*variant, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // More keys/values?
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;
    }
  }

  bool skipObject(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
    if (!skipSpacesAndComments())
      return false;

    // Empty object?
    if (eat('}'))
      return true;

    // Read each key value pair
    for (;;) {
      // Skip key
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // Colon
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      // Skip value
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // More keys/values?
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  bool parseKey() {
    _stringStorage.startString();
    if (isQuote(current())) {
      return parseQuotedString();
    } else {
      return parseNonQuotedString();
    }
  }

  bool parseStringValue(VariantData &variant) {
    _stringStorage.startString();
    if (!parseQuotedString())
      return false;
    const char *value = _stringStorage.save();
    variant.setStringPointer(value, typename TStringStorage::storage_policy());
    return true;
  }

  bool parseQuotedString() {
#if ARDUINOJSON_DECODE_UNICODE
    Utf16::Codepoint codepoint;
#endif
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;

      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }

      if (c == '\\') {
        c = current();

        if (c == '\0') {
          _error = DeserializationError::IncompleteInput;
          return false;
        }

        if (c == 'u') {
#if ARDUINOJSON_DECODE_UNICODE
          move();
          uint16_t codeunit;
          if (!parseHex4(codeunit))
            return false;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), _stringStorage);
#else
          _stringStorage.append('\\');
#endif
          continue;
        }

        // replace char
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0') {
          _error = DeserializationError::InvalidInput;
          return false;
        }
        move();
      }

      _stringStorage.append(c);
    }

    _stringStorage.append('\0');

    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    return true;
  }

  bool parseNonQuotedString() {
    char c = current();
    ARDUINOJSON_ASSERT(c);

    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        _stringStorage.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      _error = DeserializationError::InvalidInput;
      return false;
    }

    _stringStorage.append('\0');

    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    return true;
  }

  bool skipString() {
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      if (c == '\\') {
        if (current() != '\0')
          move();
      }
    }

    return true;
  }

  bool parseNumericValue(VariantData &result) {
    uint8_t n = 0;

    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      _buffer[n++] = c;
      c = current();
    }
    _buffer[n] = 0;

    c = _buffer[0];
    if (c == 't') {  // true
      result.setBoolean(true);
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'f') {  // false
      result.setBoolean(false);
      if (n != 5) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'n') {  // null
      // the variant is already null
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }

    if (!parseNumber(_buffer, result)) {
      _error = DeserializationError::InvalidInput;
      return false;
    }

    return true;
  }

  bool skipNumericValue() {
    char c = current();
    while (canBeInNonQuotedString(c)) {
      move();
      c = current();
    }
    return true;
  }

  bool parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      uint8_t value = decodeHex(digit);
      if (value > 0x0F) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
      result = uint16_t((result << 4) | value);
      move();
    }
    return true;
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

  static inline uint8_t decodeHex(char c) {
    if (c < 'A')
      return uint8_t(c - '0');
    c = char(c & ~0x20);  // uppercase
    return uint8_t(c - 'A' + 10);
  }

  bool skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        // end of string
        case '\0':
          _error = _foundSomething ? DeserializationError::IncompleteInput
                                   : DeserializationError::EmptyInput;
          return false;

        // spaces
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          move();
          continue;

#if ARDUINOJSON_ENABLE_COMMENTS
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
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
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
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
                if (c == '\n')
                  break;
              }
              break;

            // not a comment, just a '/'
            default:
              _error = DeserializationError::InvalidInput;
              return false;
          }
          break;
#endif

        default:
          _foundSomething = true;
          return true;
      }
    }
  }

  TStringStorage _stringStorage;
  bool _foundSomething;
  Latch<TReader> _latch;
  MemoryPool *_pool;
  char _buffer[64];  // using a member instead of a local variable because it
                     // ended in the recursive path after compiler inlined the
                     // code
  DeserializationError _error;
};

//
// deserializeJson(JsonDocument&, const std::string&, ...)
//
// ... = NestingLimit
template <typename TString>
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TString>
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TString>
DeserializationError deserializeJson(JsonDocument &doc, const TString &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

//
// deserializeJson(JsonDocument&, std::istream&, ...)
//
// ... = NestingLimit
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TStream>
DeserializationError deserializeJson(JsonDocument &doc, TStream &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

//
// deserializeJson(JsonDocument&, char*, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

//
// deserializeJson(JsonDocument&, char*, size_t, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
// ... = NestingLimit, Filter
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     size_t inputSize,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}

}  // namespace ARDUINOJSON_NAMESPACE
