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

JsonValue JsonArray::add() { return JsonValue(_impl ? _impl->add() : NULL); }

JsonValue JsonArray::operator[](int index) const {
  return JsonValue(_impl ? (*_impl)[index] : NULL);
}

JsonArray JsonArray::createNestedArray() {
  return JsonArray(_impl ? _impl->createNestedArray() : NULL);
}

JsonObject JsonArray::createNestedObject() {
  return JsonObject(_impl ? _impl->createNestedObject() : NULL);
}