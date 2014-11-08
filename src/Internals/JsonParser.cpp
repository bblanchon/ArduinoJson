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

    case '{':
      destination = parseObject();
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

    case '\'':
    case '\"':
      destination = parseString();
      break;
  }

  _nestingLimit++;
}

JsonArray &JsonParser::parseArray() {
  // Create an empty array
  JsonArray &array = _buffer->createArray();

  // Check opening braket
  if (!skip('[')) goto ERROR_MISSING_BRACKET;
  if (skip(']')) goto SUCCESS_EMPTY_ARRAY;

  // Read each value
  for (;;) {
    // 1 - Parse value
    JsonVariant &value = array.add();
    parseAnythingTo(value);
    if (!value.success()) goto ERROR_INVALID_VALUE;

    // 2 - More values?
    if (skip(']')) goto SUCCES_NON_EMPTY_ARRAY;
    if (!skip(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_ARRAY:
SUCCES_NON_EMPTY_ARRAY:
  return array;

ERROR_INVALID_VALUE:
ERROR_MISSING_BRACKET:
ERROR_MISSING_COMMA:
  return JsonArray::invalid();
}

JsonObject &JsonParser::parseObject() {
  // Create an empty object
  JsonObject &object = _buffer->createObject();

  // Check opening brace
  if (!skip('{')) goto ERROR_MISSING_BRACE;
  if (skip('}')) goto SUCCESS_EMPTY_OBJECT;

  // Read each key value pair
  for (;;) {
    // 1 - Parse key
    const char *key = parseString();
    if (!key) goto ERROR_INVALID_KEY;
    if (!skip(':')) goto ERROR_MISSING_COLON;

    // 2 - Parse value
    JsonVariant &value = object.add(key);
    parseAnythingTo(value);
    if (!value.success()) goto ERROR_INVALID_VALUE;

    // 3 - More keys/values?
    if (skip('}')) goto SUCCESS_NON_EMPTY_OBJECT;
    if (!skip(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_OBJECT:
SUCCESS_NON_EMPTY_OBJECT:
  return object;

ERROR_INVALID_KEY:
ERROR_INVALID_VALUE:
ERROR_MISSING_BRACE:
ERROR_MISSING_COLON:
ERROR_MISSING_COMMA:
  return JsonObject::invalid();
}

void JsonParser::parseBooleanTo(JsonVariant &destination) {
  if (skip("true"))
    destination = true;
  else if (skip("false"))
    destination = false;
  else
    destination = JsonVariant::invalid();
}

void JsonParser::parseNumberTo(JsonVariant &destination) {
  char *endOfLong;
  long longValue = strtol(_ptr, &endOfLong, 10);
  char stopChar = *endOfLong;

  // Could it be a floating point value?
  bool couldBeFloat = stopChar == '.' || stopChar == 'e' || stopChar == 'E';

  if (couldBeFloat) {
    // Yes => parse it as a double
    double doubleValue = strtod(_ptr, &_ptr);
    // Count the decimal digits
    uint8_t decimals = static_cast<uint8_t>(_ptr - endOfLong - 1);
    // Set the variant as a double
    destination.set(doubleValue, decimals);
  } else {
    // No => set the variant as a long
    _ptr = endOfLong;
    destination = longValue;
  }
}

void JsonParser::parseNullTo(JsonVariant &destination) {
  const char *NULL_STRING = NULL;
  if (skip("null"))
    destination = NULL_STRING;
  else
    destination = JsonVariant::invalid();
}

const char *JsonParser::parseString() {
  return QuotedString::extractFrom(_ptr, &_ptr);
}
