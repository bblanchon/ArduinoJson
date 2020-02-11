// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Json/EscapeSequence.hpp>
#include <ArduinoJson/Json/Utf16.hpp>
#include <ArduinoJson/Json/Utf8.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class JsonDeserializer {
  typedef typename remove_reference<TStringStorage>::type::StringBuilder
      StringBuilder;

 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage, uint8_t nestingLimit)
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _nestingLimit(nestingLimit),
        _loaded(false) {
#ifdef ARDUINOJSON_DEBUG
    _ended = false;
#endif
  }

  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter) {
    DeserializationError err = parseVariant(variant, filter);

    if (!err && _current != 0 && !variant.isEnclosed()) {
      // We don't detect trailing characters earlier, so we need to check now
      err = DeserializationError::InvalidInput;
    }

    return err;
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

  char current() {
    if (!_loaded) {
      ARDUINOJSON_ASSERT(!_ended);
      int c = _reader.read();
#ifdef ARDUINOJSON_DEBUG
      if (c <= 0) _ended = true;
#endif
      _current = static_cast<char>(c > 0 ? c : 0);
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

  template <typename TFilter>
  DeserializationError parseVariant(VariantData &variant, TFilter filter) {
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    switch (current()) {
      case '[':
        if (filter.allowArray())
          return parseArray(variant.toArray(), filter);
        else
          return skipArray();

      case '{':
        if (filter.allowObject())
          return parseObject(variant.toObject(), filter);
        else
          return skipObject();

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

  DeserializationError skipVariant() {
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    switch (current()) {
      case '[':
        return skipArray();

      case '{':
        return skipObject();

      case '\"':
      case '\'':
        return skipString();

      default:
        return skipNumericValue();
    }
  }

  template <typename TFilter>
  DeserializationError parseArray(CollectionData &array, TFilter filter) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    // Check opening braket
    if (!eat('[')) return DeserializationError::InvalidInput;

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    // Empty array?
    if (eat(']')) return DeserializationError::Ok;

    TFilter memberFilter = filter[0UL];

    // Read each value
    for (;;) {
      if (memberFilter.allow()) {
        // Allocate slot in array
        VariantData *value = array.add(_pool);
        if (!value) return DeserializationError::NoMemory;

        // 1 - Parse value
        _nestingLimit--;
        err = parseVariant(*value, memberFilter);
        _nestingLimit++;
        if (err) return err;
      } else {
        _nestingLimit--;
        err = skipVariant();
        _nestingLimit++;
        if (err) return err;
      }

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // 3 - More values?
      if (eat(']')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
    }
  }

  DeserializationError skipArray() {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    // Check opening braket
    if (!eat('[')) return DeserializationError::InvalidInput;

    // Read each value
    for (;;) {
      // 1 - Skip value
      _nestingLimit--;
      DeserializationError err = skipVariant();
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

  template <typename TFilter>
  DeserializationError parseObject(CollectionData &object, TFilter filter) {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

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
      err = parseKey(key);
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;  // Colon
      if (!eat(':')) return DeserializationError::InvalidInput;

      TFilter memberFilter = filter[key];

      if (memberFilter.allow()) {
        VariantData *variant = object.get(adaptString(key));
        if (!variant) {
          // Allocate slot in object
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot) return DeserializationError::NoMemory;

          slot->setOwnedKey(make_not_null(key));

          variant = slot->data();
        }

        // Parse value
        _nestingLimit--;
        err = parseVariant(*variant, memberFilter);
        _nestingLimit++;
        if (err) return err;
      } else {
        _stringStorage.reclaim(key);
        _nestingLimit--;
        err = skipVariant();
        _nestingLimit++;
        if (err) return err;
      }

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

  DeserializationError skipObject() {
    if (_nestingLimit == 0) return DeserializationError::TooDeep;

    // Check opening brace
    if (!eat('{')) return DeserializationError::InvalidInput;

    // Skip spaces
    DeserializationError err = skipSpacesAndComments();
    if (err) return err;

    // Empty object?
    if (eat('}')) return DeserializationError::Ok;

    // Read each key value pair
    for (;;) {
      // Skip key
      err = skipVariant();
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;  // Colon
      if (!eat(':')) return DeserializationError::InvalidInput;

      // Skip value
      _nestingLimit--;
      err = skipVariant();
      _nestingLimit++;
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // More keys/values?
      if (eat('}')) return DeserializationError::Ok;
      if (!eat(',')) return DeserializationError::InvalidInput;
    }
  }

  DeserializationError parseKey(const char *&key) {
    if (isQuote(current())) {
      return parseQuotedString(key);
    } else {
      return parseNonQuotedString(key);
    }
  }

  DeserializationError parseStringValue(VariantData &variant) {
    const char *value;
    DeserializationError err = parseQuotedString(value);
    if (err) return err;
    variant.setOwnedString(make_not_null(value));
    return DeserializationError::Ok;
  }

  DeserializationError parseQuotedString(const char *&result) {
    StringBuilder builder = _stringStorage.startString();
#if ARDUINOJSON_DECODE_UNICODE
    Utf16::Codepoint codepoint;
#endif
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar) break;

      if (c == '\0') return DeserializationError::IncompleteInput;

      if (c == '\\') {
        c = current();
        if (c == '\0') return DeserializationError::IncompleteInput;
        if (c == 'u') {
#if ARDUINOJSON_DECODE_UNICODE
          move();
          uint16_t codeunit;
          DeserializationError err = parseHex4(codeunit);
          if (err) return err;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), builder);
          continue;
#else
          return DeserializationError::NotSupported;
#endif
        }
        // replace char
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0') return DeserializationError::InvalidInput;
        move();
      }

      builder.append(c);
    }

    result = builder.complete();
    if (!result) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }

  DeserializationError parseNonQuotedString(const char *&result) {
    StringBuilder builder = _stringStorage.startString();

    char c = current();
    if (c == '\0') return DeserializationError::IncompleteInput;

    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        builder.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      return DeserializationError::InvalidInput;
    }

    result = builder.complete();
    if (!result) return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }

  DeserializationError skipString() {
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar) break;
      if (c == '\0') return DeserializationError::IncompleteInput;
      if (c == '\\') _reader.read();
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
    while (c && c != '}' && c != ',' && c != ']' && c != ':') {
      move();
      c = current();
    }
    return DeserializationError::Ok;
  }

  DeserializationError parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit) return DeserializationError::IncompleteInput;
      uint8_t value = decodeHex(digit);
      if (value > 0x0F) return DeserializationError::InvalidInput;
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
    if (c < 'A') return uint8_t(c - '0');
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
#endif

        default:
          return DeserializationError::Ok;
      }
    }
  }

  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  uint8_t _nestingLimit;
  char _current;
  bool _loaded;
#ifdef ARDUINOJSON_DEBUG
  bool _ended;
#endif
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
