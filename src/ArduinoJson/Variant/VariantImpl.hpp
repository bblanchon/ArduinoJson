// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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
  return JsonArray(getPool(), variantToArray(getOrCreateData(), getPool()));
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type
VariantRefBase<TDerived>::to() const {
  return JsonObject(getPool(), variantToObject(getOrCreateData(), getPool()));
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type
VariantRefBase<TDerived>::to() const {
  variantSetNull(getOrCreateData(), getPool());
  return *this;
}

template <typename TDerived>
inline void convertToJson(const VariantRefBase<TDerived>& src,
                          JsonVariant dst) {
  dst.set(src.template as<JsonVariantConst>());
}

inline void VariantSlot::setKey(StringNode* k) {
  ARDUINOJSON_ASSERT(k);
  flags_ |= OWNED_KEY_BIT;
  key_ = k->data;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
