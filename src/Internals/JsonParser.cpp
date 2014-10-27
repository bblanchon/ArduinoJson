// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h>  // for strtol, strtod
#include <ctype.h>

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

void JsonParser::skipSpaces() {
  while (isspace(*_ptr)) _ptr++;
}

bool JsonParser::skip(char charToSkip) {
  skipSpaces();
  if (*_ptr != charToSkip) return false;
  _ptr++;
  skipSpaces();
  return true;
}

void JsonParser::parseValueTo(JsonValue destination) {
  skipSpaces();

  switch (*_ptr) {
    case '[':
      destination = parseArray();
      break;

    case 't':
    case 'f':
      parseBooleanTo(destination);
      break;

    case '-':
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      parseNumberTo(destination);
      break;

    case 'n':
      parseNullTo(destination);
      break;

    case '{':
      destination = parseObject();
      break;

    case '\'':
    case '\"':
      destination = parseString();
      break;
  }
}

JsonArray JsonParser::parseArray() {
  skip('[');

  if (isEnd()) return NULL;

  JsonArray array = _buffer->createArray();
  if (skip(']')) return array;  // empty array

  for (;;) {
    JsonValue child = array.add();

    parseValueTo(child);
    if (!child.success()) return NULL;

    if (skip(']')) return array;  // end of the array

    if (!skip(',')) return NULL;  // comma is missing
  }
}

void JsonParser::parseBooleanTo(JsonValue &destination) {
  bool value = *_ptr == 't';

  // TODO: bug if string ends here !!!

  _ptr += value ? 4 : 5;
  // 4 = strlen("true")
  // 5 = strlen("false");

  destination = value;
}

void JsonParser::parseNumberTo(JsonValue &destination) {
  char *endOfLong;
  long longValue = strtol(_ptr, &endOfLong, 10);

  if (*endOfLong == '.') {
    // stopped on a decimal separator
    double douleValue = strtod(_ptr, &_ptr);
    int decimals = _ptr - endOfLong - 1;
    destination.set(douleValue, decimals);
  } else {
    _ptr = endOfLong;
    destination = longValue;
  }
}

void JsonParser::parseNullTo(JsonValue &destination) {
  _ptr += 4;  // strlen("null")

  destination = static_cast<const char *>(NULL);
}

JsonObject JsonParser::parseObject() {
  skip('{');

  if (isEnd()) return NULL;  // premature ending

  JsonObject object = _buffer->createObject();

  if (skip('}')) return object;  // empty object

  for (;;) {
    const char *key = parseString();
    if (!key) return NULL;

    if (!skip(':')) return NULL;

    JsonValue value = object[key];

    parseValueTo(value);
    if (!value.success()) return NULL;

    if (skip('}')) return object;  // end of the object

    if (!skip(',')) return 0;  // comma is missing
  }
}

const char *JsonParser::parseString() {
  return QuotedString::extractFrom(_ptr, &_ptr);
}

JsonValue JsonParser::parseValue() {
  JsonValue value = _buffer->createValue();
  parseValueTo(value);
  return value;
}
