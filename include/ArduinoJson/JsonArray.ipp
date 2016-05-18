// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonArraySubscript.hpp"

namespace ArduinoJson {

inline JsonArraySubscript JsonArray::operator[](size_t index) {
  return JsonArraySubscript(*this, index);
}

inline JsonVariant JsonArray::operator[](size_t index) const {
  return get(index);
}

template <typename TValue>
inline bool JsonArray::addNode(TValue value) {
  node_type *node = addNewNode();
  return node != NULL && setNodeValue<TValue>(node, value);
}

template <typename TValue>
inline bool JsonArray::setNodeAt(size_t index, TValue value) {
  node_type *node = getNodeAt(index);
  return node != NULL && setNodeValue<TValue>(node, value);
}

template <typename TValue>
inline bool JsonArray::setNodeValue(node_type *node, TValue value) {
  node->content = value;
  return true;
}

template <>
inline bool JsonArray::setNodeValue(node_type *node, String &value) {
  const char *copy = _buffer->strdup(value);
  if (!copy) return false;
  node->content = copy;
  return true;
}

inline JsonVariant JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content : JsonVariant();
}

template <typename T>
inline T JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.as<T>() : JsonVariant::defaultValue<T>();
}

template <typename T>
inline bool JsonArray::is(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.is<T>() : false;
}

template <typename TImplem>
inline const JsonArraySubscript JsonVariantBase<TImplem>::operator[](
    int index) const {
  return asArray()[index];
}

template <>
inline JsonArray &JsonVariant::defaultValue<JsonArray &>() {
  return JsonArray::invalid();
}

template <>
inline JsonArray const &JsonVariant::defaultValue<JsonArray const &>() {
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
