// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h>  // for strtol, strtod
#include <ctype.h>

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/QuotedString.hpp"

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

JsonNode *JsonParser::parseAnything() {
  skipSpaces();

  switch (*_ptr) {
    case '[':
      return parseArray();

    case 't':
    case 'f':
      return parseBoolean();

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
      return parseNumber();

    case 'n':
      return parseNull();

    case '{':
      return parseObject();

    case '\'':
    case '\"':
      return parseString();

    default:
      return NULL;  // invalid JSON
  }
}

JsonNode *JsonParser::parseArray() {
  JsonNode *node = _buffer->createArrayNode();

  skip('[');

  if (isEnd()) return 0;

  if (skip(']')) return node;  // empty array

  for (;;) {
    JsonNode *child = parseAnything();

    if (!child) return 0;  // child parsing failed

    node->addChild(child);

    if (skip(']')) return node;  // end of the array

    if (!skip(',')) return 0;  // comma is missing
  }
}

JsonNode *JsonParser::parseBoolean() {
  bool value = *_ptr == 't';

  _ptr += value ? 4 : 5;
  // 4 = strlen("true")
  // 5 = strlen("false");

  return _buffer->createBoolNode(value);
}

JsonNode *JsonParser::parseNumber() {
  char *endOfLong;
  long longValue = strtol(_ptr, &endOfLong, 10);

  if (*endOfLong == '.') {
    // stopped on a decimal separator
    double value = strtod(_ptr, &_ptr);
    int decimals = _ptr - endOfLong - 1;
    return _buffer->createDoubleNode(value, decimals);
  } else {
    _ptr = endOfLong;
    return _buffer->createLongNode(longValue);
  }
}

JsonNode *JsonParser::parseNull() {
  _ptr += 4;  // strlen("null")

  return _buffer->createStringNode(0);
}

JsonNode *JsonParser::parseObject() {
  JsonNode *node = _buffer->createObjectNode();

  skip('{');

  if (isEnd()) return 0;  // premature ending

  if (skip('}')) return node;  // empty object

  for (;;) {
    JsonNode *child = parseObjectKeyValue();

    if (!child) return 0;  // child parsing failed

    node->addChild(child);

    if (skip('}')) return node;  // end of the object

    if (!skip(',')) return 0;  // comma is missing
  }
}

JsonNode *JsonParser::parseObjectKeyValue() {
  const char *key = QuotedString::extractFrom(_ptr, &_ptr);

  if (!key) return 0;  // failed to extract key

  if (!skip(':')) return 0;  // colon is missing

  JsonNode *value = parseAnything();

  if (!value) return 0;  // value parsing failed

  return _buffer->createObjectKeyValueNode(key, value);
}

JsonNode *JsonParser::parseString() {
  const char *s = QuotedString::extractFrom(_ptr, &_ptr);
  return _buffer->createStringNode(s);
}
