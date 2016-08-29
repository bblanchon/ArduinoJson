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

inline JsonVariant::JsonVariant(JsonArray &array) {
  if (array.success()) {
    _type = Internals::JSON_ARRAY;
    _content.asArray = &array;
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

inline JsonVariant::JsonVariant(JsonObject &object) {
  if (object.success()) {
    _type = Internals::JSON_OBJECT;
    _content.asObject = &object;
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

template <>
inline bool JsonArray::setNodeValue(node_type *node, String &value) {
  const char *copy = _buffer->strdup(value);
  if (!copy) return false;
  node->content = copy;
  return true;
}

template <>
inline JsonArray &JsonVariant::defaultValue<JsonArray>() {
  return JsonArray::invalid();
}

template <>
inline JsonArray &JsonVariant::defaultValue<JsonArray &>() {
  return JsonArray::invalid();
}

template <>
inline const JsonArray &JsonVariant::defaultValue<const JsonArray>() {
  return JsonArray::invalid();
}

template <>
inline const JsonArray &JsonVariant::defaultValue<const JsonArray &>() {
  return JsonArray::invalid();
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

inline JsonArray &JsonObject::createNestedArray(JsonObjectKey key) {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  setNodeAt<const JsonVariant &>(key, array);
  return array;
}
}
