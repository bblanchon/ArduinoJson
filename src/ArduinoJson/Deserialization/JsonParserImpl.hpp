// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Data/Encoding.hpp"
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
  if (_nestingLimit == 0) return false;
  _nestingLimit--;
  bool success = parseAnythingToUnsafe(destination);
  _nestingLimit++;
  return success;
}

template <typename TReader, typename TWriter>
inline bool
ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseAnythingToUnsafe(
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
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(
    JsonArray &array) {
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
  return true;

ERROR_INVALID_VALUE:
ERROR_MISSING_BRACKET:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return false;
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(
    JsonObject &object) {
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
  return true;

ERROR_INVALID_KEY:
ERROR_INVALID_VALUE:
ERROR_MISSING_BRACE:
ERROR_MISSING_COLON:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return false;
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parse(
    JsonVariant &variant) {
  return parseAnythingTo(&variant);
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseArrayTo(
    JsonVariant *destination) {
  JsonArray *array = new (_buffer) JsonArray(_buffer);
  if (!array) return false;
  *destination = array;
  return parse(*array);
}

template <typename TReader, typename TWriter>
inline bool ArduinoJson::Internals::JsonParser<TReader, TWriter>::parseObjectTo(
    JsonVariant *destination) {
  JsonObject *object = new (_buffer) JsonObject(_buffer);
  if (!object) return false;
  *destination = object;
  return parse(*object);
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
        c = Encoding::unescapeChar(_reader.current());
        if (c == '\0') break;
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
