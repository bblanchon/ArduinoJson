// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariant.hpp"
#include "Internals/Parse.hpp"
#include "JsonArray.hpp"
#include "JsonObject.hpp"

#include <string.h>  // for strcmp
#include <errno.h>   // for errno
#include <stdlib.h>  // for strtol, strtod

namespace ArduinoJson {

inline Internals::JsonInteger JsonVariant::asInteger() const {
  using namespace Internals;
  switch (_type) {
    case JSON_UNDEFINED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return _content.asInteger;
    case JSON_NEGATIVE_INTEGER:
      return -static_cast<Internals::JsonInteger>(_content.asInteger);
    case JSON_STRING:
    case JSON_UNPARSED:
      if (!_content.asString) return 0;
      if (!strcmp("true", _content.asString)) return 1;
      return parse<Internals::JsonInteger>(_content.asString);
    default:
      return static_cast<Internals::JsonInteger>(_content.asFloat);
  }
}

inline Internals::JsonUInt JsonVariant::asUnsignedInteger() const {
  using namespace Internals;
  switch (_type) {
    case JSON_UNDEFINED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
    case JSON_NEGATIVE_INTEGER:
      return _content.asInteger;
    case JSON_STRING:
    case JSON_UNPARSED:
      if (!_content.asString) return 0;
      if (!strcmp("true", _content.asString)) return 1;
      return parse<Internals::JsonUInt>(_content.asString);
    default:
      return static_cast<Internals::JsonUInt>(_content.asFloat);
  }
}

inline const char *JsonVariant::asString() const {
  using namespace Internals;
  if (_type == JSON_UNPARSED && _content.asString &&
      !strcmp("null", _content.asString))
    return NULL;
  if (_type == JSON_STRING || _type == JSON_UNPARSED) return _content.asString;
  return NULL;
}

inline Internals::JsonFloat JsonVariant::asFloat() const {
  using namespace Internals;
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

inline String JsonVariant::toString() const {
  using namespace Internals;
  String s;
  if ((_type == JSON_STRING || _type == JSON_UNPARSED) &&
      _content.asString != NULL)
    s = _content.asString;
  else
    printTo(s);
  return s;
}

inline bool JsonVariant::isBoolean() const {
  using namespace Internals;
  if (_type == JSON_BOOLEAN) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  return !strcmp(_content.asString, "true") ||
         !strcmp(_content.asString, "false");
}

inline bool JsonVariant::isInteger() const {
  using namespace Internals;
  if (_type == JSON_POSITIVE_INTEGER || _type == JSON_NEGATIVE_INTEGER)
    return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtol(_content.asString, &end, 10);

  return *end == '\0' && errno == 0;
}

inline bool JsonVariant::isFloat() const {
  using namespace Internals;
  if (_type >= JSON_FLOAT_0_DECIMALS) return true;

  if (_type != JSON_UNPARSED || _content.asString == NULL) return false;

  char *end;
  errno = 0;
  strtod(_content.asString, &end);

  return *end == '\0' && errno == 0 && !is<long>();
}

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream &operator<<(std::ostream &os, const JsonVariant &source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson
