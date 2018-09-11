// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariant.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline bool JsonVariant::set(JsonArray array) {
  return to<JsonArray>().copyFrom(array);
}

inline bool JsonVariant::set(const Internals::JsonArraySubscript& value) {
  return set(value.as<JsonVariant>());
}

inline bool JsonVariant::set(JsonObject object) {
  return to<JsonObject>().copyFrom(object);
}

template <typename TString>
inline bool JsonVariant::set(
    const Internals::JsonObjectSubscript<TString>& value) {
  return set(value.template as<JsonVariant>());
}

inline bool JsonVariant::set(const JsonVariant& value) {
  if (!_data) return false;
  if (!value._data) {
    _data->setNull();
    return true;
  }
  switch (value._data->type) {
    case Internals::JSON_ARRAY:
      return set(value.as<JsonArray>());
    case Internals::JSON_OBJECT:
      return set(value.as<JsonObject>());
    case Internals::JSON_OWNED_STRING:
      return set(const_cast<char*>(value._data->content.asString));
    case Internals::JSON_OWNED_RAW:
      return set(serialized(const_cast<char*>(value._data->content.asRaw.data),
                            value._data->content.asRaw.size));
    default:
      *_data = *value._data;
      return true;
  }
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariant::as() const {
  return _data ? JsonArray(_memoryPool, _data->asArray()) : JsonArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariant::as() const {
  return _data ? JsonObject(_memoryPool, _data->asObject()) : JsonObject();
}

template <typename T>
inline typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                     JsonArray>::type
JsonVariant::to() {
  if (!_data) return JsonArray();
  return JsonArray(_memoryPool, _data->toArray());
}

template <typename T>
typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                              JsonObject>::type
JsonVariant::to() {
  if (!_data) return JsonObject();
  return JsonObject(_memoryPool, _data->toObject());
}

template <typename T>
typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                              JsonVariant>::type
JsonVariant::to() {
  if (!_data) return JsonVariant();
  _data->setNull();
  return *this;
}

template <typename Visitor>
inline void JsonVariant::accept(Visitor& visitor) const {
  using namespace Internals;
  if (!_data) return visitor.visitNull();

  switch (_data->type) {
    case JSON_FLOAT:
      return visitor.visitFloat(_data->content.asFloat);

    case JSON_ARRAY:
      return visitor.visitArray(
          JsonArray(_memoryPool, &_data->content.asArray));

    case JSON_OBJECT:
      return visitor.visitObject(
          JsonObject(_memoryPool, &_data->content.asObject));

    case JSON_LINKED_STRING:
    case JSON_OWNED_STRING:
      return visitor.visitString(_data->content.asString);

    case JSON_LINKED_RAW:
    case JSON_OWNED_RAW:
      return visitor.visitRawJson(_data->content.asRaw.data,
                                  _data->content.asRaw.size);

    case JSON_NEGATIVE_INTEGER:
      return visitor.visitNegativeInteger(_data->content.asInteger);

    case JSON_POSITIVE_INTEGER:
      return visitor.visitPositiveInteger(_data->content.asInteger);

    case JSON_BOOLEAN:
      return visitor.visitBoolean(_data->content.asInteger != 0);

    default:
      return visitor.visitNull();
  }
}
}  // namespace ArduinoJson
