// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"

using namespace ArduinoJson;

JsonValue JsonObject::operator[](char const *key) {
  return JsonValue(_impl ? (*_impl)[key] : NULL);
}

JsonArray JsonObject::createNestedArray(key_type key) {
  return JsonArray(_impl ? _impl->createNestedArray(key) : NULL);
}

JsonObject JsonObject::createNestedObject(key_type key) {
  return JsonObject(_impl ? _impl->createNestedObject(key) : NULL);
}
