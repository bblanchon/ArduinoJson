// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Comments.hpp"
#include "JsonParser.hpp"

inline bool ArduinoJson::Internals::JsonParser::eat(StringReader &reader,
                                                    char charToSkip) {
  skipSpacesAndComments(reader);
  if (reader.peek() != charToSkip) return false;
  reader.skip();
  skipSpacesAndComments(reader);
  return true;
}

inline bool ArduinoJson::Internals::JsonParser::parseAnythingTo(
    JsonVariant *destination) {
  if (_nestingLimit == 0) return false;
  _nestingLimit--;
  bool success = parseAnythingToUnsafe(destination);
  _nestingLimit++;
  return success;
}

inline bool ArduinoJson::Internals::JsonParser::parseAnythingToUnsafe(
    JsonVariant *destination) {
  skipSpacesAndComments(_reader);

  switch (_reader.peek()) {
    case '[':
      return parseArrayTo(destination);

    case '{':
      return parseObjectTo(destination);

    default:
      return parseStringTo(destination);
  }
}

inline ArduinoJson::JsonArray &
ArduinoJson::Internals::JsonParser::parseArray() {
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
  return array;

ERROR_INVALID_VALUE:
ERROR_MISSING_BRACKET:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonArray::invalid();
}

inline bool ArduinoJson::Internals::JsonParser::parseArrayTo(
    JsonVariant *destination) {
  JsonArray &array = parseArray();
  if (!array.success()) return false;

  *destination = array;
  return true;
}

inline ArduinoJson::JsonObject &
ArduinoJson::Internals::JsonParser::parseObject() {
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
  return object;

ERROR_INVALID_KEY:
ERROR_INVALID_VALUE:
ERROR_MISSING_BRACE:
ERROR_MISSING_COLON:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonObject::invalid();
}

inline bool ArduinoJson::Internals::JsonParser::parseObjectTo(
    JsonVariant *destination) {
  JsonObject &object = parseObject();
  if (!object.success()) return false;

  *destination = object;
  return true;
}

inline const char *ArduinoJson::Internals::JsonParser::parseString() {
  const char *str = _writer.startString();

  char c = _reader.peek();

  if (isQuote(c)) {  // quotes
    _reader.skip();
    char stopChar = c;
    for (;;) {
      c = _reader.peek();
      if (c == '\0') break;
      _reader.skip();

      if (c == stopChar) break;

      if (c == '\\') {
        // replace char
        c = Encoding::unescapeChar(_reader.peek());
        if (c == '\0') break;
        _reader.skip();
      }

      _writer.append(c);
    }
  } else {  // no quotes
    for (;;) {
      if (!isLetterOrNumber(c)) break;
      _reader.skip();
      _writer.append(c);
      c = _reader.peek();
    }
  }

  _writer.stopString();
  return str;
}

inline bool ArduinoJson::Internals::JsonParser::parseStringTo(
    JsonVariant *destination) {
  bool hasQuotes = isQuote(_reader.peek());
  const char *value = parseString();
  if (value == NULL) return false;
  if (hasQuotes) {
    *destination = value;
  } else {
    *destination = RawJson(value);
  }
  return true;
}
