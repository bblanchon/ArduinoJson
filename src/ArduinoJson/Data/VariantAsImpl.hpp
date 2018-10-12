// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonVariant.hpp"
#include "../Serialization/DynamicStringWriter.hpp"
#include "VariantFunctions.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline typename enable_if<is_same<JsonVariantConst, T>::value, T>::type
variantAs(const JsonVariantData* _data) {
  return JsonVariantConst(_data);
}

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const JsonVariantData* _data) {
  const char* cstr = variantAsString(_data);
  if (cstr) return T(cstr);
  T s;
  serializeJson(JsonVariantConst(_data), s);
  return s;
}

}  // namespace ARDUINOJSON_NAMESPACE
