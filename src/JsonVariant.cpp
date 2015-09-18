// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonVariant.hpp"

#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

#include <errno.h>   // for errno
#include <stdlib.h>  // for strtol, strtod

using namespace ArduinoJson::Internals;

namespace ArduinoJson {

template <>
const char *JsonVariant::as<const char *>() const {
  if (_type == JSON_UNPARSED && _content.asString &&
      !strcmp("null", _content.asString))
    return NULL;
  if (_type == JSON_STRING || _type == JSON_UNPARSED) return _content.asString;
  return NULL;
}

template <>
double JsonVariant::as<double>() const {
  if (_type >= JSON_DOUBLE_0_DECIMALS) return _content.asDouble;

  if (_type == JSON_LONG || _type == JSON_BOOLEAN)
    return static_cast<double>(_content.asLong);

  if ((_type == JSON_STRING || _type == JSON_UNPARSED) && _content.asString)
    return strtod(_content.asString, NULL);

  return 0.0;
}

template <>
long JsonVariant::as<long>() const {
  if (_type == JSON_LONG || _type == JSON_BOOLEAN) return _content.asLong;

  if (_type >= JSON_DOUBLE_0_DECIMALS)
    return static_cast<long>(_content.asDouble);

  if ((_type == JSON_STRING || _type == JSON_UNPARSED) && _content.asString) {
    if (!strcmp("true", _content.asString)) return 1;
    return strtol(_content.asString, NULL, 10);
  }

  return 0L;
}

template <>
String JsonVariant::as<String>() const {
  if ((_type == JSON_STRING || _type == JSON_UNPARSED) &&
      _content.asString != NULL)
    return String(_content.asString);

  if (_type == JSON_LONG || _type == JSON_BOOLEAN)
    return String(_content.asLong);

  if (_type >= JSON_DOUBLE_0_DECIMALS) {
    uint8_t decimals = static_cast<uint8_t>(_type - JSON_DOUBLE_0_DECIMALS);
    return String(_content.asDouble, decimals);
  }

  String s;
  printTo(s);
  return s;
}

template <>
bool JsonVariant::is<signed long>() const {
  if (_type == JSON_LONG) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtol(_content.asString, &end, 10);

  return *end == '\0' && errno == 0;
}

template <>
bool JsonVariant::is<double>() const {
  if (_type >= JSON_DOUBLE_0_DECIMALS) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtod(_content.asString, &end);

  return *end == '\0' && errno == 0 && !is<long>();
}

void JsonVariant::writeTo(JsonWriter &writer) const {
  if (_type == JSON_ARRAY)
    _content.asArray->writeTo(writer);

  else if (_type == JSON_OBJECT)
    _content.asObject->writeTo(writer);

  else if (_type == JSON_STRING)
    writer.writeString(_content.asString);

  else if (_type == JSON_UNPARSED)
    writer.writeRaw(_content.asString);

  else if (_type == JSON_LONG)
    writer.writeLong(_content.asLong);

  else if (_type == JSON_BOOLEAN)
    writer.writeBoolean(_content.asLong != 0);

  else if (_type >= JSON_DOUBLE_0_DECIMALS) {
    uint8_t decimals = static_cast<uint8_t>(_type - JSON_DOUBLE_0_DECIMALS);
    writer.writeDouble(_content.asDouble, decimals);
  }
}
}
