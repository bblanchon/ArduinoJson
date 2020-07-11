// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
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
      : _stringStorage(stringStorage), _latch(reader), _pool(&pool) {}

  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    DeserializationError err = parseVariant(variant, filter, nestingLimit);

    if (!err && _latch.last() != 0 && !variant.isEnclosed()) {
      // We don't detect trailing characters earlier, so we need to check now
      err = DeserializationError::InvalidInput;
    }

    return err;
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

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
  DeserializationError parseVariant(VariantData &variant, TFilter filter,
                                    NestingLimit nestingLimit) {
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

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

  DeserializationError skipVariant(NestingLimit nestingLimit) {
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

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
  DeserializationError parseArray(CollectionData &array, TFilter filter,
                                  NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty array?
    if (eat(']'))
      return DeserializationError::Ok;

    TFilter memberFilter = filter[0UL];

    // Read each value
    for (;;) {
      if (memberFilter.allow()) {
        // Allocate slot in array
        VariantData *value = array.addElement(_pool);
        if (!value)
          return DeserializationError::NoMemory;

        // 1 - Parse value
        err = parseVariant(*value, memberFilter, nestingLimit.decrement());
        if (err)
          return err;
      } else {
        err = skipVariant(nestingLimit.decrement());
        if (err)
          return err;
      }

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // 3 - More values?
      if (eat(']'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  DeserializationError skipArray(NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Read each value
    for (;;) {
      // 1 - Skip value
      DeserializationError err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // 3 - More values?
      if (eat(']'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  template <typename TFilter>
  DeserializationError parseObject(CollectionData &object, TFilter filter,
                                   NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty object?
    if (eat('}'))
      return DeserializationError::Ok;

    // Read each key value pair
    for (;;) {
      _stringStorage.startString(_pool);

      // Parse key
      err = parseKey();
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;  // Colon
      if (!eat(':'))
        return DeserializationError::InvalidInput;

      const char *key = _stringStorage.c_str();

      TFilter memberFilter = filter[key];

      if (memberFilter.allow()) {
        VariantData *variant = object.getMember(adaptString(key));
        if (!variant) {
          _stringStorage.commit(_pool);

          // Allocate slot in object
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot)
            return DeserializationError::NoMemory;

          slot->setOwnedKey(make_not_null(key));

          variant = slot->data();
        }

        // Parse value
        err = parseVariant(*variant, memberFilter, nestingLimit.decrement());
        if (err)
          return err;
      } else {
        err = skipVariant(nestingLimit.decrement());
        if (err)
          return err;
      }

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // More keys/values?
      if (eat('}'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;
    }
  }

  DeserializationError skipObject(NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty object?
    if (eat('}'))
      return DeserializationError::Ok;

    // Read each key value pair
    for (;;) {
      // Skip key
      err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // Colon
      if (!eat(':'))
        return DeserializationError::InvalidInput;

      // Skip value
      err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // More keys/values?
      if (eat('}'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  DeserializationError parseKey() {
    if (isQuote(current())) {
      return parseQuotedString();
    } else {
      return parseNonQuotedString();
    }
  }

  DeserializationError parseStringValue(VariantData &variant) {
    _stringStorage.startString(_pool);
    DeserializationError err = parseQuotedString();
    if (err)
      return err;
    _stringStorage.commit(_pool);
    variant.setOwnedString(make_not_null(_stringStorage.c_str()));
    return DeserializationError::Ok;
  }

  DeserializationError parseQuotedString() {
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

      if (c == '\0')
        return DeserializationError::IncompleteInput;

      if (c == '\\') {
        c = current();
        if (c == '\0')
          return DeserializationError::IncompleteInput;
        if (c == 'u') {
#if ARDUINOJSON_DECODE_UNICODE
          move();
          uint16_t codeunit;
          DeserializationError err = parseHex4(codeunit);
          if (err)
            return err;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), _stringStorage);
          continue;
#else
          return DeserializationError::NotSupported;
#endif
        }
        // replace char
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0')
          return DeserializationError::InvalidInput;
        move();
      }

      _stringStorage.append(c);
    }

    _stringStorage.append('\0');

    if (!_stringStorage.isValid())
      return DeserializationError::NoMemory;

    return DeserializationError::Ok;
  }

  DeserializationError parseNonQuotedString() {
    char c = current();
    ARDUINOJSON_ASSERT(c);

    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        _stringStorage.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      return DeserializationError::InvalidInput;
    }

    _stringStorage.append('\0');

    if (!_stringStorage.isValid())
      return DeserializationError::NoMemory;

    return DeserializationError::Ok;
  }

  DeserializationError skipString() {
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0')
        return DeserializationError::IncompleteInput;
      if (c == '\\') {
        if (current() != '\0')
          move();
      }
    }

    return DeserializationError::Ok;
  }

  DeserializationError parseNumericValue(VariantData &result) {
    char buffer[64];
    uint8_t n = 0;

    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      buffer[n++] = c;
      c = current();
    }
    buffer[n] = 0;

    c = buffer[0];
    if (c == 't') {  // true
      result.setBoolean(true);
      return n == 4 ? DeserializationError::Ok
                    : DeserializationError::IncompleteInput;
    }
    if (c == 'f') {  // false
      result.setBoolean(false);
      return n == 5 ? DeserializationError::Ok
                    : DeserializationError::IncompleteInput;
    }
    if (c == 'n') {  // null
      // the variant is already null
      return n == 4 ? DeserializationError::Ok
                    : DeserializationError::IncompleteInput;
    }

    ParsedNumber<Float, UInt> num = parseNumber<Float, UInt>(buffer);

    switch (num.type()) {
      case VALUE_IS_NEGATIVE_INTEGER:
        result.setNegativeInteger(num.uintValue);
        return DeserializationError::Ok;

      case VALUE_IS_POSITIVE_INTEGER:
        result.setPositiveInteger(num.uintValue);
        return DeserializationError::Ok;

      case VALUE_IS_FLOAT:
        result.setFloat(num.floatValue);
        return DeserializationError::Ok;
    }

    return DeserializationError::InvalidInput;
  }

  DeserializationError skipNumericValue() {
    char c = current();
    while (canBeInNonQuotedString(c)) {
      move();
      c = current();
    }
    return DeserializationError::Ok;
  }

  DeserializationError parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit)
        return DeserializationError::IncompleteInput;
      uint8_t value = decodeHex(digit);
      if (value > 0x0F)
        return DeserializationError::InvalidInput;
      result = uint16_t((result << 4) | value);
      move();
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

  static inline uint8_t decodeHex(char c) {
    if (c < 'A')
      return uint8_t(c - '0');
    c = char(c & ~0x20);  // uppercase
    return uint8_t(c - 'A' + 10);
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
                if (c == '\0')
                  return DeserializationError::IncompleteInput;
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
                if (c == '\0')
                  return DeserializationError::IncompleteInput;
                if (c == '\n')
                  break;
              }
              break;

            // not a comment, just a '/'
            default:
              return DeserializationError::InvalidInput;
          }
          break;
#endif

        default:
          return DeserializationError::Ok;
      }
    }
  }

  TStringStorage _stringStorage;
  Latch<TReader> _latch;
  MemoryPool *_pool;
};

// deserializeJson(JsonDocument&, const std::string&, ...)
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, const TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, const TInput &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TInput>
DeserializationError deserializeJson(JsonDocument &doc, const TInput &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

// deserializeJson(JsonDocument&, const std::istream&, ...)
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, TInput &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TInput>
DeserializationError deserializeJson(JsonDocument &doc, TInput &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

// deserializeJson(JsonDocument&, char*, ...)
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

// deserializeJson(JsonDocument&, char*, size_t, ...)
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     size_t inputSize,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}

}  // namespace ARDUINOJSON_NAMESPACE
