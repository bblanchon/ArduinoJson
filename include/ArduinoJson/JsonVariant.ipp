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

template <typename T>
inline T JsonVariant::invalid() {
  return T();
}

inline Internals::JsonInteger JsonVariant::asInteger() const {
  if (_type == Internals::JSON_INTEGER || _type == Internals::JSON_BOOLEAN)
    return _content.asInteger;

  if (_type >= Internals::JSON_FLOAT_0_DECIMALS)
    return static_cast<Internals::JsonInteger>(_content.asFloat);

  if ((_type == Internals::JSON_STRING || _type == Internals::JSON_UNPARSED) &&
      _content.asString) {
    if (!strcmp("true", _content.asString)) return 1;
    return Internals::parse<Internals::JsonInteger>(_content.asString);
  }

  return 0L;
}

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream &operator<<(std::ostream &os, const JsonVariant &source) {
  return source.printTo(os);
}
#endif

}  // namespace ArduinoJson
