// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>

#include <string.h>  // for strcmp

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return content_.asBoolean;
    case VALUE_IS_UNSIGNED_INTEGER:
      return convertNumber<T>(content_.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return convertNumber<T>(content_.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(content_.asString.data);
    case VALUE_IS_FLOAT:
      return convertNumber<T>(content_.asFloat);
    default:
      return 0;
  }
}

inline bool VariantData::asBoolean() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return content_.asBoolean;
    case VALUE_IS_SIGNED_INTEGER:
    case VALUE_IS_UNSIGNED_INTEGER:
      return content_.asUnsignedInteger != 0;
    case VALUE_IS_FLOAT:
      return content_.asFloat != 0;
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
      return static_cast<T>(content_.asBoolean);
    case VALUE_IS_UNSIGNED_INTEGER:
      return static_cast<T>(content_.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return static_cast<T>(content_.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(content_.asString.data);
    case VALUE_IS_FLOAT:
      return static_cast<T>(content_.asFloat);
    default:
      return 0;
  }
}

inline JsonString VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
      return JsonString(content_.asString.data, content_.asString.size,
                        JsonString::Linked);
    case VALUE_IS_OWNED_STRING:
      return JsonString(content_.asString.data, content_.asString.size,
                        JsonString::Copied);
    default:
      return JsonString();
  }
}

inline bool VariantData::copyFrom(const VariantData& src, MemoryPool* pool) {
  switch (src.type()) {
    case VALUE_IS_ARRAY:
      return toArray().copyFrom(src.content_.asCollection, pool);
    case VALUE_IS_OBJECT:
      return toObject().copyFrom(src.content_.asCollection, pool);
    case VALUE_IS_OWNED_STRING: {
      JsonString value = src.asString();
      return setString(adaptString(value), pool);
    }
    case VALUE_IS_OWNED_RAW:
      return storeOwnedRaw(
          serialized(src.content_.asString.data, src.content_.asString.size),
          pool);
    default:
      setType(src.type());
      content_ = src.content_;
      return true;
  }
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::add() const {
  return JsonVariant(getPool(),
                     variantAddElement(getOrCreateData(), getPool()));
}

template <typename TDerived>
template <typename T>
inline typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
VariantRefBase<TDerived>::as() const {
  return Converter<T>::fromJson(getVariant());
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
inline typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
VariantRefBase<TDerived>::is() const {
  return Converter<T>::checkJson(getVariant());
}

template <typename TDerived>
template <typename T>
inline bool VariantRefBase<TDerived>::set(const T& value) const {
  Converter<typename detail::remove_cv<T>::type>::toJson(value,
                                                         getOrCreateVariant());
  MemoryPool* pool = getPool();
  return pool && !pool->overflowed();
}

template <typename TDerived>
template <typename T>
inline bool VariantRefBase<TDerived>::set(T* value) const {
  Converter<T*>::toJson(value, getOrCreateVariant());
  MemoryPool* pool = getPool();
  return pool && !pool->overflowed();
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
  auto data = getOrCreateData();
  variantSetNull(data);
  return JsonVariant(getPool(), data);
}

template <typename TDerived>
inline void convertToJson(const VariantRefBase<TDerived>& src,
                          JsonVariant dst) {
  dst.set(src.template as<JsonVariantConst>());
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
