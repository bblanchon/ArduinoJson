// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline typename enable_if<is_same<ArrayConstRef, T>::value, T>::type variantAs(
    const VariantData* _data) {
  return ArrayConstRef(variantAsArray(_data));
}

template <typename T>
inline typename enable_if<is_same<ObjectConstRef, T>::value, T>::type variantAs(
    const VariantData* _data) {
  return ObjectConstRef(variantAsObject(_data));
}

template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data) {
  return VariantConstRef(_data);
}

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data) {
  const char* cstr = _data != 0 ? _data->asString() : 0;
  if (cstr)
    return T(cstr);
  T s;
  serializeJson(VariantConstRef(_data), s);
  return s;
}

template <>
inline ArrayRef variantAs<ArrayRef>(VariantData* data, MemoryPool* pool) {
  return ArrayRef(pool, data != 0 ? data->asArray() : 0);
}

template <>
inline ObjectRef variantAs<ObjectRef>(VariantData* data, MemoryPool* pool) {
  return ObjectRef(pool, data != 0 ? data->asObject() : 0);
}

template <>
inline VariantRef variantAs<VariantRef>(VariantData* data, MemoryPool* pool) {
  return VariantRef(pool, data);
}

}  // namespace ARDUINOJSON_NAMESPACE
