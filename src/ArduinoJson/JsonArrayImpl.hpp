// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline JsonArray JsonArray::createNestedArray() const {
  return add().to<JsonArray>();
}

inline JsonObject JsonArray::createNestedObject() const {
  return add().to<JsonObject>();
}
}  // namespace ARDUINOJSON_NAMESPACE
