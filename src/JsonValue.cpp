// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonValue.hpp"

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;

JsonValue::operator JsonArray() const {
  return _impl ? JsonArray(*_impl) : JsonArray();
}

JsonValue::operator JsonObject() const {
  return _impl ? JsonObject(*_impl) : JsonObject();
}

void JsonValue::operator=(JsonArray array) {
  if (_impl) _impl->set(array._impl);
}

void JsonValue::operator=(JsonObject object) {
  if (_impl) _impl->set(object._impl);
}