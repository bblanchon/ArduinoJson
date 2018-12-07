// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Serialization/DynamicStringWriter.hpp"
#include "VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data) {
  return VariantConstRef(_data);
}

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data) {
  const char* cstr = _data != 0 ? _data->asString() : 0;
  if (cstr) return T(cstr);
  T s;
  serializeJson(VariantConstRef(_data), s);
  return s;
}

}  // namespace ARDUINOJSON_NAMESPACE
