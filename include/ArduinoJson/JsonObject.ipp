// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonObjectSubscript.hpp"

namespace ArduinoJson {

inline JsonVariant JsonObject::get(JsonObjectKey key) const {
  node_type *node = getNodeAt(key.c_str());
  return node ? node->content.value : JsonVariant();
}

template <typename T>
inline T JsonObject::get(JsonObjectKey key) const {
  node_type *node = getNodeAt(key.c_str());
  return node ? node->content.value.as<T>() : JsonVariant::defaultValue<T>();
}

template <typename T>
inline bool JsonObject::is(JsonObjectKey key) const {
  node_type *node = getNodeAt(key.c_str());
  return node ? node->content.value.is<T>() : false;
}

inline JsonObjectSubscript<const char *> JsonObject::operator[](
    const char *key) {
  return JsonObjectSubscript<const char *>(*this, key);
}

inline JsonObjectSubscript<const String &> JsonObject::operator[](
    const String &key) {
  return JsonObjectSubscript<const String &>(*this, key);
}

inline JsonVariant JsonObject::operator[](JsonObjectKey key) const {
  return get(key);
}

inline bool JsonObject::containsKey(JsonObjectKey key) const {
  return getNodeAt(key.c_str()) != NULL;
}

inline void JsonObject::remove(JsonObjectKey key) {
  removeNode(getNodeAt(key.c_str()));
}

template <typename T>
inline bool JsonObject::setNodeAt(JsonObjectKey key, T value) {
  node_type *node = getNodeAt(key.c_str());
  if (!node) {
    node = addNewNode();
    if (!node || !setNodeKey(node, key)) return false;
  }
  return setNodeValue<T>(node, value);
}

inline bool JsonObject::setNodeKey(node_type *node, JsonObjectKey key) {
  if (key.needs_copy()) {
    node->content.key = _buffer->strdup(key.c_str());
    if (node->content.key == NULL) return false;
  } else {
    node->content.key = key.c_str();
  }
  return true;
}

template <typename TValue>
inline bool JsonObject::setNodeValue(node_type *node, TValue value) {
  node->content.value = value;
  return true;
}

template <>
inline bool JsonObject::setNodeValue(node_type *node, String &value) {
  node->content.value = _buffer->strdup(value);
  return node->content.value;
}

template <>
inline bool JsonObject::setNodeValue(node_type *node, const String &value) {
  node->content.value = _buffer->strdup(value);
  return node->content.value;
}

template <typename TImplem>
inline const JsonObjectSubscript<const char *> JsonVariantBase<TImplem>::
operator[](const char *key) const {
  return asObject()[key];
}

template <typename TImplem>
inline const JsonObjectSubscript<const String &> JsonVariantBase<TImplem>::
operator[](const String &key) const {
  return asObject()[key];
}

template <>
inline JsonObject const &JsonVariant::defaultValue<JsonObject const &>() {
  return JsonObject::invalid();
}

template <>
inline JsonObject &JsonVariant::defaultValue<JsonObject &>() {
  return JsonObject::invalid();
}

inline JsonObject &JsonVariant::asObject() const {
  if (_type == Internals::JSON_OBJECT) return *_content.asObject;
  return JsonObject::invalid();
}

inline JsonObject &JsonObject::createNestedObject(JsonObjectKey key) {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &array = _buffer->createObject();
  setNodeAt<const JsonVariant &>(key, array);
  return array;
}

inline JsonObject &JsonArray::createNestedObject() {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &object = _buffer->createObject();
  add(object);
  return object;
}
}
