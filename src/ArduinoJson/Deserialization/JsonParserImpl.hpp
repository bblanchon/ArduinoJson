// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Comments.hpp"
#include "JsonParser.hpp"

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::eat(
    TReader &reader, char charToSkip) {
  skipSpacesAndComments(reader);
  if (reader.current() != charToSkip) return false;
  reader.move();
  return true;
}

template <typename TReader, typename TWriter>
inline bool
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseAnythingTo(
    JsonVariant *destination) {
  skipSpacesAndComments(_reader);

  switch (_reader.current()) {
    case '[':
      return parseArrayTo(destination);

    case '{':
      return parseObjectTo(destination);

    default:
      return parseStringTo(destination);
  }
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonArray &
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseArray() {
  if (_nestingLimit == 0) return JsonArray::invalid();
  _nestingLimit--;

  // Create an empty array
  JsonArray &array = _buffer->createArray();

  // Check opening braket
  if (!eat('[')) goto ERROR_MISSING_BRACKET;
  if (eat(']')) goto SUCCESS_EMPTY_ARRAY;

  // Read each value
  for (;;) {
    // 1 - Parse value
    JsonVariant value;
    if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
    if (!array.add(value)) goto ERROR_NO_MEMORY;

    // 2 - More values?
    if (eat(']')) goto SUCCES_NON_EMPTY_ARRAY;
    if (!eat(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_ARRAY:
SUCCES_NON_EMPTY_ARRAY:
  _nestingLimit++;
  return array;

ERROR_INVALID_VALUE:
ERROR_MISSING_BRACKET:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonArray::invalid();
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseArrayTo(
    JsonVariant *destination) {
  JsonArray &array = parseArray();
  if (!array.success()) return false;

  *destination = array;
  return true;
}

template <typename TReader, typename TWriter>
inline ArduinoJson::JsonObject &
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseObject() {
  if (_nestingLimit == 0) return JsonObject::invalid();
  _nestingLimit--;

  // Create an empty object
  JsonObject &object = _buffer->createObject();

  // Check opening brace
  if (!eat('{')) goto ERROR_MISSING_BRACE;
  if (eat('}')) goto SUCCESS_EMPTY_OBJECT;

  // Read each key value pair
  for (;;) {
    // 1 - Parse key
    const char *key = parseString();
    if (!key) goto ERROR_INVALID_KEY;
    if (!eat(':')) goto ERROR_MISSING_COLON;

    // 2 - Parse value
    JsonVariant value;
    if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
    if (!object.set(key, value)) goto ERROR_NO_MEMORY;

    // 3 - More keys/values?
    if (eat('}')) goto SUCCESS_NON_EMPTY_OBJECT;
    if (!eat(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_OBJECT:
SUCCESS_NON_EMPTY_OBJECT:
  _nestingLimit++;
  return object;

ERROR_INVALID_KEY:
ERROR_INVALID_VALUE:
ERROR_MISSING_BRACE:
ERROR_MISSING_COLON:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonObject::invalid();
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseObjectTo(
    JsonVariant *destination) {
  JsonObject &object = parseObject();
  if (!object.success()) return false;

  *destination = object;
  return true;
}

template <typename Str>
static inline void encode_to_utf8(const uint16_t codepoint, Str &str) {
  if (codepoint >= 0x00000800) {
    str.append(static_cast<char>(0xe0 /*0b11100000*/ | (codepoint >> 12)));
    str.append(static_cast<char>(((codepoint >> 6) & 0x3f /*0b00111111*/) | 0x80));
    str.append(static_cast<char>((codepoint & 0x3f /*0b00111111*/) | 0x80));
  } else if (codepoint >= 0x00000080) {
    str.append(static_cast<char>(0xc0 /*0b11000000*/ | (codepoint >> 6)));
    str.append(static_cast<char>((codepoint & 0x3f /*0b00111111*/) | 0x80));
  } else {
    str.append(static_cast<char>(codepoint));
  }
}

static inline char parse_hexdigit(char c) {
  if (c < 'A')
      return static_cast<char>(c - '0');
  c &= ~0x20;
  return static_cast<char>(c - 'A' + 10);
}

template <typename TReader, typename TWriter>
inline const char *
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseString() {
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
        const char escaped = _reader.current();
        if (escaped == 'u') {
          uint16_t codepoint = 0;
          char i=0;
          while(1)
          {
              _reader.move();
              if (i >=4)
                  break;
              codepoint = static_cast<uint16_t>((codepoint << 4) | parse_hexdigit(_reader.current()));
              ++i;
          }
          encode_to_utf8(codepoint, str);
          continue;
        } else {
          c = Encoding::unescapeChar(escaped);
          if (c == '\0') break;
        }
        _reader.move();
      }

      str.append(c);
    }
  } else {  // no quotes
    for (;;) {
      if (!canBeInNonQuotedString(c)) break;
      _reader.move();
      str.append(c);
      c = _reader.current();
    }
  }

  return str.c_str();
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseStringTo(
    JsonVariant *destination) {
  bool hasQuotes = isQuote(_reader.current());
  const char *value = parseString();
  if (value == NULL) return false;
  if (hasQuotes) {
    *destination = value;
  } else {
    *destination = RawJson(value);
  }
  return true;
}
