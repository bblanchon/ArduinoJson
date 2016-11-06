// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonArray.hpp"
#include "JsonArraySubscript.hpp"
#include "JsonObject.hpp"

namespace ArduinoJson {

inline JsonVariant::JsonVariant(const JsonArray &array) {
  if (array.success()) {
    _type = Internals::JSON_ARRAY;
    _content.asArray = const_cast<JsonArray *>(&array);
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

inline JsonVariant::JsonVariant(const JsonObject &object) {
  if (object.success()) {
    _type = Internals::JSON_OBJECT;
    _content.asObject = const_cast<JsonObject *>(&object);
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

namespace Internals {
template <>
struct JsonVariantDefault<JsonArray> {
  static JsonArray &get() {
    return JsonArray::invalid();
  }
};
}

inline JsonArray &JsonVariant::asArray() const {
  if (_type == Internals::JSON_ARRAY) return *_content.asArray;
  return JsonArray::invalid();
}

inline JsonArray &JsonArray::createNestedArray() {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  add(array);
  return array;
}

template <typename TString>
inline JsonArray &JsonObject::createNestedArray(const TString &key) {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  set(key, array);
  return array;
}
}
