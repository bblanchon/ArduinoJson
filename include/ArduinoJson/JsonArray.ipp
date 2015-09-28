// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonArray.hpp"
#include "JsonArraySubscript.hpp"

namespace ArduinoJson {

inline JsonArraySubscript JsonArray::operator[](size_t index) {
  return JsonArraySubscript(*this, index);
}

inline JsonVariant JsonArray::operator[](size_t index) const {
  return get(index);
}

inline bool JsonArray::add(bool value) { return addNode<bool>(value); }

inline bool JsonArray::add(float value, uint8_t decimals) {
  return addNode<const JsonVariant &>(JsonVariant(value, decimals));
}

inline bool JsonArray::add(double value, uint8_t decimals) {
  return addNode<const JsonVariant &>(JsonVariant(value, decimals));
}

inline bool JsonArray::add(signed char value) {
  return addNode<signed char>(value);
}

inline bool JsonArray::add(signed long value) {
  return addNode<signed long>(value);
}

inline bool JsonArray::add(signed int value) {
  return addNode<signed int>(value);
}

inline bool JsonArray::add(signed short value) {
  return addNode<signed short>(value);
}

inline bool JsonArray::add(unsigned char value) {
  return addNode<unsigned char>(value);
}

inline bool JsonArray::add(unsigned long value) {
  return addNode<unsigned long>(value);
}

inline bool JsonArray::add(unsigned int value) {
  return addNode<unsigned int>(value);
}

inline bool JsonArray::add(unsigned short value) {
  return addNode<unsigned short>(value);
}

inline bool JsonArray::add(const char *value) {
  return addNode<const char *>(value);
}

inline bool JsonArray::add(const String &value) {
  return addNode<const String &>(value);
}

inline bool JsonArray::add(JsonArray &array) {
  return addNode<JsonArray &>(array);
}

inline bool JsonArray::add(JsonObject &object) {
  return addNode<JsonObject &>(object);
}

template <typename T>
inline bool JsonArray::add(const T &variant) {
  return addNode<const JsonVariant &>(variant);
}

template <typename TValue>
inline bool JsonArray::addNode(TValue value) {
  node_type *node = addNewNode();
  if (node == NULL) return false;
  setNodeValue<TValue>(node, value);
  return true;
}

inline void JsonArray::set(size_t index, bool value) {
  return setNodeAt<bool>(index, value);
}

inline void JsonArray::set(size_t index, float value, uint8_t decimals) {
  return setNodeAt<const JsonVariant &>(index, JsonVariant(value, decimals));
}

inline void JsonArray::set(size_t index, double value, uint8_t decimals) {
  return setNodeAt<const JsonVariant &>(index, JsonVariant(value, decimals));
}

inline void JsonArray::set(size_t index, signed char value) {
  return setNodeAt<signed char>(index, value);
}

inline void JsonArray::set(size_t index, signed long value) {
  return setNodeAt<signed long>(index, value);
}

inline void JsonArray::set(size_t index, signed int value) {
  return setNodeAt<signed int>(index, value);
}

inline void JsonArray::set(size_t index, signed short value) {
  return setNodeAt<signed short>(index, value);
}

inline void JsonArray::set(size_t index, unsigned char value) {
  return setNodeAt<unsigned char>(index, value);
}

inline void JsonArray::set(size_t index, unsigned long value) {
  return setNodeAt<unsigned long>(index, value);
}

inline void JsonArray::set(size_t index, unsigned int value) {
  return setNodeAt<unsigned int>(index, value);
}

inline void JsonArray::set(size_t index, unsigned short value) {
  return setNodeAt<unsigned short>(index, value);
}

inline void JsonArray::set(size_t index, const char *value) {
  return setNodeAt<const char *>(index, value);
}

inline void JsonArray::set(size_t index, const String &value) {
  return setNodeAt<const String &>(index, value);
}

inline void JsonArray::set(size_t index, JsonArray &array) {
  return setNodeAt<JsonArray &>(index, array);
}

inline void JsonArray::set(size_t index, JsonObject &object) {
  return setNodeAt<JsonObject &>(index, object);
}

template <typename T>
inline void JsonArray::set(size_t index, const T &variant) {
  return setNodeAt<const JsonVariant &>(index, variant);
}

template <typename TValue>
inline void JsonArray::setNodeAt(size_t index, TValue value) {
  node_type *node = getNodeAt(index);
  if (node == NULL) return;
  setNodeValue<TValue>(node, value);
}

template <typename TValue>
inline void JsonArray::setNodeValue(node_type *node, TValue value) {
  node->content = value;
}

template <>
inline void JsonArray::setNodeValue(node_type *node, const String &value) {
  node->content = _buffer->strdup(value);
}

inline JsonVariant JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content : JsonVariant();
}

template <typename T>
inline T JsonArray::get(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.as<T>() : JsonVariant::invalid<T>();
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
inline JsonArray &JsonVariant::invalid<JsonArray &>() {
  return JsonArray::invalid();
}

template <>
inline JsonArray const &JsonVariant::invalid<JsonArray const &>() {
  return JsonArray::invalid();
}

template <>
inline JsonArray &JsonVariant::as<JsonArray &>() const {
  if (_type == Internals::JSON_ARRAY) return *_content.asArray;
  return JsonArray::invalid();
}

template <>
inline const JsonArray &JsonVariant::as<const JsonArray &>() const {
  if (_type == Internals::JSON_ARRAY) return *_content.asArray;
  return JsonArray::invalid();
}
}
