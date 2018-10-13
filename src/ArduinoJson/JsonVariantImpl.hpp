// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariant.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ARDUINOJSON_NAMESPACE {

inline bool JsonVariant::set(JsonArray array) const {
  return to<JsonArray>().copyFrom(array);
}

inline bool JsonVariant::set(const JsonArraySubscript& value) const {
  return set(value.as<JsonVariant>());
}

inline bool JsonVariant::set(JsonObject object) const {
  return to<JsonObject>().copyFrom(object);
}

template <typename TString>
inline bool JsonVariant::set(const JsonObjectSubscript<TString>& value) const {
  return set(value.template as<JsonVariant>());
}

inline bool JsonVariant::set(JsonVariantConst value) const {
  return variantCopy(_data, value._data, _memoryPool);
}

inline bool JsonVariant::set(JsonVariant value) const {
  return variantCopy(_data, value._data, _memoryPool);
}

template <typename T>
inline typename enable_if<is_same<T, JsonArray>::value, T>::type
JsonVariant::as() const {
  return JsonArray(_memoryPool, variantAsArray(_data));
}

template <typename T>
inline typename enable_if<is_same<T, JsonObject>::value, T>::type
JsonVariant::as() const {
  return JsonObject(_memoryPool, variantAsObject(_data));
}

template <typename T>
inline typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type
JsonVariant::to() const {
  return JsonArray(_memoryPool, variantToArray(_data));
}

template <typename T>
typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type
JsonVariant::to() const {
  return JsonObject(_memoryPool, variantToObject(_data));
}

template <typename T>
typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type
JsonVariant::to() const {
  variantSetNull(_data);
  return *this;
}

template <typename Visitor>
inline void JsonVariant::accept(Visitor& visitor) const {
  return JsonVariantConst(_data).accept(visitor);
}

template <typename Visitor>
inline void JsonVariantConst::accept(Visitor& visitor) const {
  if (!_data) return visitor.visitNull();

  switch (_data->type) {
    case JSON_FLOAT:
      return visitor.visitFloat(_data->content.asFloat);

    case JSON_ARRAY:
      return visitor.visitArray(JsonArrayConst(&_data->content.asArray));

    case JSON_OBJECT:
      return visitor.visitObject(JsonObjectConst(&_data->content.asObject));

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

inline JsonVariantConst JsonVariantConst::operator[](size_t index) const {
  return JsonArrayConst(variantAsArray(_data))[index];
}

}  // namespace ARDUINOJSON_NAMESPACE
