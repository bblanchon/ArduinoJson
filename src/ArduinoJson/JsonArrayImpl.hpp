// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ArduinoJson {

inline JsonArray JsonArray::createNestedArray() {
  if (!_data) return JsonArray();
  JsonArray array(_data->_buffer);
  if (!array.isNull()) add(array);
  return array;
}

inline JsonObject JsonArray::createNestedObject() {
  if (!_data) return JsonObject();
  JsonObject object(_data->_buffer);
  if (!object.isNull()) add(object);
  return object;
}
}  // namespace ArduinoJson
