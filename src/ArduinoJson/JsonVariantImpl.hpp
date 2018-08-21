// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonArrayData.hpp"
#include "JsonObjectData.hpp"
#include "JsonVariant.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline bool JsonVariant::set(const JsonArray& array) {
  if (!_data) return false;
  if (array._data)
    _data->setArray(*array._data);
  else
    _data->setNull();
  return true;
}

inline bool JsonVariant::set(const Internals::JsonArraySubscript& value) {
  return set(value.as<JsonVariant>());
}

inline bool JsonVariant::set(const JsonObject& object) {
  if (!_data) return false;
  if (object._data)
    _data->setObject(*object._data);
  else
    _data->setNull();
  return true;
}

template <typename TString>
inline bool JsonVariant::set(
    const Internals::JsonObjectSubscript<TString>& value) {
  return set(value.template as<JsonVariant>());
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariant::as() const {
  return _data ? JsonArray(_buffer, _data->asArray()) : JsonArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariant::as() const {
  return _data ? JsonObject(_buffer, _data->asObject()) : JsonObject();
}
}  // namespace ArduinoJson
