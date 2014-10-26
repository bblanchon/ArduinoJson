// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonValue.hpp"

#include "ArduinoJson/JsonArray.hpp"

using namespace ArduinoJson;

JsonValue::operator JsonArray() const {
  return _impl ? JsonArray(*_impl) : JsonArray();
}
