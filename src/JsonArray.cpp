// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValue JsonArray::operator[](int index) const {
  if (!_impl) return JsonValue::null();
  return JsonValue((*_impl)[index]);
}

JsonArray JsonArray::createNestedArray() {
  if (!_impl) return JsonArray::null();
  return JsonArray(_impl->createNestedArray());
}

JsonObject JsonArray::createNestedObject() {
  if (!_impl) return JsonObject::null();
  return JsonObject(_impl->createNestedObject()));
}