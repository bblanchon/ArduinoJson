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

#include <string.h>

namespace ArduinoJson {

inline JsonVariant::JsonVariant(bool value) {
  using namespace Internals;
  _type = JSON_BOOLEAN;
  _content.asInteger = static_cast<JsonInteger>(value);
}

inline JsonVariant::JsonVariant(const char *value) {
  _type = Internals::JSON_STRING;
  _content.asString = value;
}

inline JsonVariant::JsonVariant(RawJson value) {
  _type = Internals::JSON_UNPARSED;
  _content.asString = value;
}

inline JsonVariant::JsonVariant(JsonArray &array) {
  _type = Internals::JSON_ARRAY;
  _content.asArray = &array;
}

inline JsonVariant::JsonVariant(JsonObject &object) {
  _type = Internals::JSON_OBJECT;
  _content.asObject = &object;
}

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

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream &operator<<(std::ostream &os, const JsonVariant &source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson
