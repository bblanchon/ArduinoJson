// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"

using namespace ArduinoJson;

JsonValue::operator JsonArray() {
  return JsonArray(_impl ? _impl->asArray() : NULL);
}

JsonValue::operator JsonObject() {
  return JsonObject(_impl ? _impl->asObject() : NULL);
}

void JsonValue::set(JsonArray array) {
  if (_impl) _impl->set(array._impl);
}

void JsonValue::set(JsonObject object) {
  if (_impl) _impl->set(object._impl);
}