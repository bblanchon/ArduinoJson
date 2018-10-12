// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Serialization/DynamicStringWriter.hpp"
#include "VariantFunctions.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonVariantConst;

template <typename T>
inline typename enable_if<is_integral<T>::value, T>::type variantAs(
    const JsonVariantData* _data) {
  return variantAsIntegral<T>(_data);
}

template <typename T>
inline typename enable_if<is_same<T, bool>::value, T>::type variantAs(
    const JsonVariantData* _data) {
  return variantAsBoolean(_data);
}

template <typename T>
inline typename enable_if<is_floating_point<T>::value, T>::type variantAs(
    const JsonVariantData* _data) {
  return variantAsFloat<T>(_data);
}

template <typename T>
inline typename enable_if<is_same<T, const char*>::value ||
                              is_same<T, char*>::value,
                          const char*>::type
variantAs(const JsonVariantData* _data) {
  return variantAsString(_data);
}

template <typename T>
inline typename enable_if<is_same<JsonVariantConst, T>::value, T>::type
variantAs(const JsonVariantData* _data);

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const JsonVariantData* _data);

}  // namespace ARDUINOJSON_NAMESPACE
