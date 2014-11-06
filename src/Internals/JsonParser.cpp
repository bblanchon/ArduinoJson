// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h>  // for strtol, strtod
#include <ctype.h>

#include "../../include/ArduinoJson/Internals/QuotedString.hpp"
#include "../../include/ArduinoJson/JsonArray.hpp"
#include "../../include/ArduinoJson/JsonBuffer.hpp"
#include "../../include/ArduinoJson/JsonObject.hpp"

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

bool JsonParser::skip(const char *wordToSkip) {
  const char *charToSkip = wordToSkip;
  while (*charToSkip && *_ptr == *charToSkip) {
    charToSkip++;
    _ptr++;
  }
  return *charToSkip == '\0';
}

void JsonParser::parseAnythingTo(JsonVariant &destination) {
  if (_nestingLimit == 0) return;
  _nestingLimit--;

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

  _nestingLimit++;
}

JsonArray &JsonParser::parseArray() {
  if (!skip('[')) return JsonArray::invalid();  // missing opening bracket

  if (isEnd()) return JsonArray::invalid();  // end of stream

  JsonArray &array = _buffer->createArray();
  if (skip(']')) return array;  // empty array

  for (;;) {
    JsonVariant &child = array.add();

    parseAnythingTo(child);
    if (!child.success()) return JsonArray::invalid();  // child parsing failed

    if (skip(']')) return array;  // end of the array

    if (!skip(',')) return JsonArray::invalid();  // comma is missing
  }
}

void JsonParser::parseBooleanTo(JsonVariant &destination) {
  bool value = *_ptr == 't';

  if (skip(value ? "true" : "false"))
    destination = value;
  else
    destination = JsonVariant::invalid();
}

void JsonParser::parseNumberTo(JsonVariant &destination) {
  char *endOfLong;
  long longValue = strtol(_ptr, &endOfLong, 10);

  if (*endOfLong == '.') {
    // stopped on a decimal separator
    double doubleValue = strtod(_ptr, &_ptr);
    uint8_t decimals = static_cast<uint8_t>(_ptr - endOfLong - 1);
    destination.set(doubleValue, decimals);
  } else {
    _ptr = endOfLong;
    destination = longValue;
  }
}

void JsonParser::parseNullTo(JsonVariant &destination) {
  if (skip("null"))
    destination = static_cast<const char *>(NULL);
  else
    destination = JsonVariant::invalid();
}

JsonObject &JsonParser::parseObject() {
  if (!skip('{')) return JsonObject::invalid();  // missing opening brace

  if (isEnd()) return JsonObject::invalid();  // premature ending

  JsonObject &object = _buffer->createObject();

  if (skip('}')) return object;  // empty object

  for (;;) {
    const char *key = parseString();
    if (!key) break;  // key parsing failed

    if (!skip(':')) break;  // colon is missing

    JsonVariant &value = object[key];

    parseAnythingTo(value);
    if (!value.success()) break;  // value parsing failed

    if (skip('}')) return object;  // end of the object

    if (!skip(',')) break;  // comma is missing
  }

  return JsonObject::invalid();
}

const char *JsonParser::parseString() {
  return QuotedString::extractFrom(_ptr, &_ptr);
}
