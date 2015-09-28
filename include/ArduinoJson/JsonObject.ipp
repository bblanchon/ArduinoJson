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
  return getNodeAt(key) != NULL;
}

inline JsonArray &JsonObject::createNestedArray(const char *key) {
  return createArrayAt<const char *>(key);
}

inline JsonArray &JsonObject::createNestedArray(const String &key) {
  return createArrayAt<const String &>(key);
}

inline JsonObject &JsonObject::createNestedObject(const char *key) {
  return createObjectAt<const char *>(key);
}

inline JsonObject &JsonObject::createNestedObject(const String &key) {
  return createObjectAt<const String &>(key);
}

inline void JsonObject::remove(JsonObjectKey key) {
  removeNode(getNodeAt(key));
}

inline bool JsonObject::set(const char *key, bool value) {
  return setNodeAt<const char *, bool>(key, value);
}
inline bool JsonObject::set(const char *key, float value, uint8_t decimals) {
  return setNodeAt<const char *, const JsonVariant &>(
      key, JsonVariant(value, decimals));
}
inline bool JsonObject::set(const char *key, double value, uint8_t decimals) {
  return setNodeAt<const char *, const JsonVariant &>(
      key, JsonVariant(value, decimals));
}
inline bool JsonObject::set(const char *key, signed char value) {
  return setNodeAt<const char *, signed char>(key, value);
}
inline bool JsonObject::set(const char *key, signed long value) {
  return setNodeAt<const char *, signed long>(key, value);
}
inline bool JsonObject::set(const char *key, signed int value) {
  return setNodeAt<const char *, signed int>(key, value);
}
inline bool JsonObject::set(const char *key, signed short value) {
  return setNodeAt<const char *, signed short>(key, value);
}
inline bool JsonObject::set(const char *key, unsigned char value) {
  return setNodeAt<const char *, unsigned char>(key, value);
}
inline bool JsonObject::set(const char *key, unsigned long value) {
  return setNodeAt<const char *, unsigned long>(key, value);
}
inline bool JsonObject::set(const char *key, unsigned int value) {
  return setNodeAt<const char *, unsigned int>(key, value);
}
inline bool JsonObject::set(const char *key, unsigned short value) {
  return setNodeAt<const char *, unsigned short>(key, value);
}
inline bool JsonObject::set(const char *key, const char *value) {
  return setNodeAt<const char *, const char *>(key, value);
}
inline bool JsonObject::set(const char *key, const String &value) {
  return setNodeAt<const char *, const String &>(key, value);
}
inline bool JsonObject::set(const char *key, JsonArray &array) {
  return setNodeAt<const char *, JsonArray &>(key, array);
}
inline bool JsonObject::set(const char *key, JsonObject &object) {
  return setNodeAt<const char *, JsonObject &>(key, object);
}
inline bool JsonObject::set(const char *key, const JsonVariant &value) {
  return setNodeAt<const char *, const JsonVariant &>(key, value);
}
template <typename T>
inline bool JsonObject::set(const char *key, const T &value) {
  return setNodeAt<const char *, JsonVariant>(key, value);
}
inline bool JsonObject::set(const String &key, bool value) {
  return setNodeAt<const String &, bool>(key, value);
}
inline bool JsonObject::set(const String &key, float value, uint8_t decimals) {
  return setNodeAt<const String &, const JsonVariant &>(
      key, JsonVariant(value, decimals));
}
inline bool JsonObject::set(const String &key, double value, uint8_t decimals) {
  return setNodeAt<const String &, const JsonVariant &>(
      key, JsonVariant(value, decimals));
}
inline bool JsonObject::set(const String &key, signed char value) {
  return setNodeAt<const String &, signed char>(key, value);
}
inline bool JsonObject::set(const String &key, signed long value) {
  return setNodeAt<const String &, signed long>(key, value);
}
inline bool JsonObject::set(const String &key, signed int value) {
  return setNodeAt<const String &, signed int>(key, value);
}
inline bool JsonObject::set(const String &key, signed short value) {
  return setNodeAt<const String &, signed short>(key, value);
}
inline bool JsonObject::set(const String &key, unsigned char value) {
  return setNodeAt<const String &, unsigned char>(key, value);
}
inline bool JsonObject::set(const String &key, unsigned long value) {
  return setNodeAt<const String &, unsigned long>(key, value);
}
inline bool JsonObject::set(const String &key, unsigned int value) {
  return setNodeAt<const String &, unsigned int>(key, value);
}
inline bool JsonObject::set(const String &key, unsigned short value) {
  return setNodeAt<const String &, unsigned short>(key, value);
}
inline bool JsonObject::set(const String &key, const char *value) {
  return setNodeAt<const String &, const char *>(key, value);
}
inline bool JsonObject::set(const String &key, const String &value) {
  return setNodeAt<const String &, const String &>(key, value);
}
inline bool JsonObject::set(const String &key, JsonArray &array) {
  return setNodeAt<const String &, JsonArray &>(key, array);
}
inline bool JsonObject::set(const String &key, JsonObject &object) {
  return setNodeAt<const String &, JsonObject &>(key, object);
}
inline bool JsonObject::set(const String &key, const JsonVariant &value) {
  return setNodeAt<const String &, const JsonVariant &>(key, value);
}
template <typename T>
inline bool JsonObject::set(const String &key, const T &value) {
  return setNodeAt<const String &, JsonVariant>(key, value);
}

template <typename TKey, typename TValue>
inline bool JsonObject::setNodeAt(TKey key, TValue value) {
  node_type *node = getOrCreateNodeAt(key);
  if (!node) return false;
  setNodeKey<TKey>(node, key);
  setNodeValue<TValue>(node, value);
  return true;
}

template <>
inline void JsonObject::setNodeKey(node_type *node, const char *key) {
  node->content.key = key;
}

template <>
inline void JsonObject::setNodeKey(node_type *node, const String &key) {
  node->content.key = _buffer->strdup(key);
}

template <typename TValue>
inline void JsonObject::setNodeValue(node_type *node, TValue value) {
  node->content.value = value;
}

template <>
inline void JsonObject::setNodeValue(node_type *node, const String &value) {
  node->content.value = _buffer->strdup(value);
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
inline JsonObject const &JsonVariant::invalid<JsonObject const &>() {
  return JsonObject::invalid();
}

template <>
inline JsonObject &JsonVariant::invalid<JsonObject &>() {
  return JsonObject::invalid();
}

template <>
inline JsonObject &JsonVariant::as<JsonObject &>() const {
  if (_type == Internals::JSON_OBJECT) return *_content.asObject;
  return JsonObject::invalid();
}

template <>
inline const JsonObject &JsonVariant::as<const JsonObject &>() const {
  if (_type == Internals::JSON_OBJECT) return *_content.asObject;
  return JsonObject::invalid();
}
}
