// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>

#include <string.h>  // for strcmp

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return _content.asBoolean;
    case VALUE_IS_UNSIGNED_INTEGER:
      return convertNumber<T>(_content.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return convertNumber<T>(_content.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString.data);
    case VALUE_IS_FLOAT:
      return convertNumber<T>(_content.asFloat);
    default:
      return 0;
  }
}

inline bool VariantData::asBoolean() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return _content.asBoolean;
    case VALUE_IS_SIGNED_INTEGER:
    case VALUE_IS_UNSIGNED_INTEGER:
      return _content.asUnsignedInteger != 0;
    case VALUE_IS_FLOAT:
      return _content.asFloat != 0;
    case VALUE_IS_NULL:
      return false;
    default:
      return true;
  }
}

// T = float/double
template <typename T>
inline T VariantData::asFloat() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return static_cast<T>(_content.asBoolean);
    case VALUE_IS_UNSIGNED_INTEGER:
      return static_cast<T>(_content.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return static_cast<T>(_content.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString.data);
    case VALUE_IS_FLOAT:
      return static_cast<T>(_content.asFloat);
    default:
      return 0;
  }
}

inline JsonString VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
      return JsonString(_content.asString.data, _content.asString.size,
                        JsonString::Linked);
    case VALUE_IS_OWNED_STRING:
      return JsonString(_content.asString.data, _content.asString.size,
                        JsonString::Copied);
    default:
      return JsonString();
  }
}

inline bool VariantData::copyFrom(const VariantData& src, MemoryPool* pool) {
  switch (src.type()) {
    case VALUE_IS_ARRAY:
      return toArray().copyFrom(src._content.asCollection, pool);
    case VALUE_IS_OBJECT:
      return toObject().copyFrom(src._content.asCollection, pool);
    case VALUE_IS_OWNED_STRING: {
      JsonString value = src.asString();
      return setString(adaptString(value), pool);
    }
    case VALUE_IS_OWNED_RAW:
      return storeOwnedRaw(
          serialized(src._content.asString.data, src._content.asString.size),
          pool);
    default:
      setType(src.type());
      _content = src._content;
      return true;
  }
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::add() const {
  return JsonVariant(getPool(),
                     variantAddElement(getOrCreateData(), getPool()));
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::getVariant() const {
  return JsonVariant(getPool(), getData());
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::getOrCreateVariant() const {
  return JsonVariant(getPool(), getOrCreateData());
}

template <typename TDerived>
template <typename T>
inline typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type
VariantRefBase<TDerived>::to() const {
  return JsonArray(getPool(), variantToArray(getOrCreateData()));
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type
VariantRefBase<TDerived>::to() const {
  return JsonObject(getPool(), variantToObject(getOrCreateData()));
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type
VariantRefBase<TDerived>::to() const {
  variantSetNull(getOrCreateData());
  return *this;
}

template <typename TDerived>
inline void convertToJson(const VariantRefBase<TDerived>& src,
                          JsonVariant dst) {
  dst.set(src.template as<JsonVariantConst>());
}

}  // namespace ARDUINOJSON_NAMESPACE
