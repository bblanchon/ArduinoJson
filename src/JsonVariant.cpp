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

template <typename TFloat>
static TFloat parse(const char *);

template <>
float parse<float>(const char *s) {
  return static_cast<float>(strtod(s, NULL));
}

template <>
double parse<double>(const char *s) {
  return strtod(s, NULL);
}

template <>
long parse<long>(const char *s) {
  return strtol(s, NULL, 10);
}

template <>
int parse<int>(const char *s) {
  return atoi(s);
}

template <>
const char *JsonVariant::as<const char *>() const {
  if (_type == JSON_UNPARSED && _content.asString &&
      !strcmp("null", _content.asString))
    return NULL;
  if (_type == JSON_STRING || _type == JSON_UNPARSED) return _content.asString;
  return NULL;
}

JsonFloat JsonVariant::asFloat() const {
  if (_type >= JSON_FLOAT_0_DECIMALS) return _content.asFloat;

  if (_type == JSON_INTEGER || _type == JSON_BOOLEAN)
    return static_cast<JsonFloat>(_content.asInteger);

  if ((_type == JSON_STRING || _type == JSON_UNPARSED) && _content.asString)
    return parse<JsonFloat>(_content.asString);

  return 0.0;
}

JsonInteger JsonVariant::asInteger() const {
  if (_type == JSON_INTEGER || _type == JSON_BOOLEAN) return _content.asInteger;

  if (_type >= JSON_FLOAT_0_DECIMALS)
    return static_cast<JsonInteger>(_content.asFloat);

  if ((_type == JSON_STRING || _type == JSON_UNPARSED) && _content.asString) {
    if (!strcmp("true", _content.asString)) return 1;
    return parse<JsonInteger>(_content.asString);
  }

  return 0L;
}

template <>
String JsonVariant::as<String>() const {
  String s;
  if ((_type == JSON_STRING || _type == JSON_UNPARSED) &&
      _content.asString != NULL)
    s = _content.asString;
  else
    printTo(s);
  return s;
}

template <>
bool JsonVariant::is<signed long>() const {
  if (_type == JSON_INTEGER) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtol(_content.asString, &end, 10);

  return *end == '\0' && errno == 0;
}

template <>
bool JsonVariant::is<double>() const {
  if (_type >= JSON_FLOAT_0_DECIMALS) return true;

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

  else if (_type == JSON_INTEGER)
    writer.writeInteger(_content.asInteger);

  else if (_type == JSON_BOOLEAN)
    writer.writeBoolean(_content.asInteger != 0);

  else if (_type >= JSON_FLOAT_0_DECIMALS) {
    uint8_t decimals = static_cast<uint8_t>(_type - JSON_FLOAT_0_DECIMALS);
    writer.writeFloat(_content.asFloat, decimals);
  }
}
}
