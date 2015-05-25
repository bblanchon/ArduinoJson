// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonObject.hpp"
#include "JsonObjectSubscript.hpp"

namespace ArduinoJson {

inline JsonVariant JsonObject::get(JsonObjectKey key) const {
  node_type *node = getNodeAt(key);
  return node ? node->content.value : JsonVariant();
}

template <typename T>
inline T JsonObject::get(JsonObjectKey key) const {
  node_type *node = getNodeAt(key);
  return node ? node->content.value.as<T>() : JsonVariant::invalid<T>();
}

template <typename T>
inline bool JsonObject::is(JsonObjectKey key) const {
  node_type *node = getNodeAt(key);
  return node ? node->content.value.is<T>() : false;
}

inline JsonObjectSubscript JsonObject::operator[](JsonObjectKey key) {
  return JsonObjectSubscript(*this, key);
}

inline const JsonObjectSubscript JsonObject::operator[](
    JsonObjectKey key) const {
  return JsonObjectSubscript(*const_cast<JsonObject *>(this), key);
}

inline bool JsonObject::containsKey(JsonObjectKey key) const {
  return getNodeAt(key) != NULL;
}

inline void JsonObject::remove(JsonObjectKey key) {
  removeNode(getNodeAt(key));
}

inline bool JsonObject::set(JsonObjectKey key, const JsonVariant value) {
  node_type *node = getOrCreateNodeAt(key);
  if (!node) return false;

  node->content.key = key;
  node->content.value = value;
  return true;
}

template <typename TImplem>
inline const JsonObjectSubscript JsonVariantBase<TImplem>::operator[](
    const char *key) const {
  return asObject()[key];
}

template <typename TImplem>
inline const JsonObjectSubscript JsonVariantBase<TImplem>::operator[](
    const String &key) const {
  return asObject()[key];
}

template <>
inline JsonObject const &JsonVariant::invalid<JsonObject const &>() {
  return JsonObject::invalid();
}

template <>
inline JsonObject &JsonVariant::invalid<JsonObject &>() {
  return JsonObject::invalid();
}
}
