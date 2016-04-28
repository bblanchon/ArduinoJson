// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../include/ArduinoJson/JsonVariant.hpp"

#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

#include <errno.h>   // for errno
#include <stdlib.h>  // for strtol, strtod

using namespace ArduinoJson::Internals;

namespace ArduinoJson {

const char *JsonVariant::asString() const {
  if (_type == JSON_UNPARSED && _content.asString &&
      !strcmp("null", _content.asString))
    return NULL;
  if (_type == JSON_STRING || _type == JSON_UNPARSED) return _content.asString;
  return NULL;
}

JsonFloat JsonVariant::asFloat() const {
  switch (_type) {
    case JSON_UNDEFINED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return static_cast<JsonFloat>(_content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return -static_cast<JsonFloat>(_content.asInteger);
    case JSON_STRING:
    case JSON_UNPARSED:
      return _content.asString ? parse<JsonFloat>(_content.asString) : 0;
    default:
      return _content.asFloat;
  }
}

String JsonVariant::toString() const {
  String s;
  if ((_type == JSON_STRING || _type == JSON_UNPARSED) &&
      _content.asString != NULL)
    s = _content.asString;
  else
    printTo(s);
  return s;
}

bool JsonVariant::isBoolean() const {
  if (_type == JSON_BOOLEAN) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  return !strcmp(_content.asString, "true") ||
         !strcmp(_content.asString, "false");
}

bool JsonVariant::isInteger() const {
  if (_type == JSON_POSITIVE_INTEGER || _type == JSON_NEGATIVE_INTEGER)
    return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtol(_content.asString, &end, 10);

  return *end == '\0' && errno == 0;
}

bool JsonVariant::isFloat() const {
  if (_type >= JSON_FLOAT_0_DECIMALS) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtod(_content.asString, &end);

  return *end == '\0' && errno == 0 && !is<long>();
}

void JsonVariant::writeTo(JsonWriter &writer) const {
  switch (_type) {
    case JSON_UNDEFINED:
      return;

    case JSON_ARRAY:
      _content.asArray->writeTo(writer);
      return;

    case JSON_OBJECT:
      _content.asObject->writeTo(writer);
      return;

    case JSON_STRING:
      writer.writeString(_content.asString);
      return;

    case JSON_UNPARSED:
      writer.writeRaw(_content.asString);
      return;

    case JSON_NEGATIVE_INTEGER:
      writer.writeRaw('-');
    case JSON_POSITIVE_INTEGER:
      writer.writeInteger(_content.asInteger);
      return;

    case JSON_BOOLEAN:
      writer.writeBoolean(_content.asInteger != 0);
      return;

    default:
      uint8_t decimals = static_cast<uint8_t>(_type - JSON_FLOAT_0_DECIMALS);
      writer.writeFloat(_content.asFloat, decimals);
  }
}
}
