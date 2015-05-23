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

inline const JsonArraySubscript JsonArray::operator[](size_t index) const {
  return JsonArraySubscript(*const_cast<JsonArray *>(this), index);
}

inline bool JsonArray::add(const JsonVariant value) {
  node_type *node = addNewNode();
  if (node) node->content = value;
  return node != NULL;
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
inline T JsonArray::is(size_t index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content.is<T>() : false;
}

inline void JsonArray::set(size_t index, const JsonVariant value) {
  node_type *node = getNodeAt(index);
  if (node) node->content = value;
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
}
