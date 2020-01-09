// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ArrayRef;
class ArrayConstRef;
class ObjectRef;
class ObjectConstRef;
class VariantRef;
class VariantConstRef;

// A metafunction that returns the type of the value returned by
// VariantRef::as<T>()
template <typename T>
struct VariantAs {
  typedef T type;
};

template <>
struct VariantAs<char*> {
  typedef const char* type;
};

// A metafunction that returns the type of the value returned by
// VariantRef::as<T>()
template <typename T>
struct VariantConstAs {
  typedef typename VariantAs<T>::type type;
};

template <>
struct VariantConstAs<VariantRef> {
  typedef VariantConstRef type;
};

template <>
struct VariantConstAs<ObjectRef> {
  typedef ObjectConstRef type;
};

template <>
struct VariantConstAs<ArrayRef> {
  typedef ArrayConstRef type;
};

// ---

template <typename T>
inline typename enable_if<is_integral<T>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asIntegral<T>() : T(0);
}

template <typename T>
inline typename enable_if<is_same<T, bool>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asBoolean() : false;
}

template <typename T>
inline typename enable_if<is_floating_point<T>::value, T>::type variantAs(
    const VariantData* _data) {
  return _data != 0 ? _data->asFloat<T>() : T(0);
}

template <typename T>
inline typename enable_if<is_same<T, const char*>::value ||
                              is_same<T, char*>::value,
                          const char*>::type
variantAs(const VariantData* _data) {
  return _data != 0 ? _data->asString() : 0;
}

template <typename T>
inline typename enable_if<is_same<ArrayConstRef, T>::value, T>::type variantAs(
    const VariantData* _data);

template <typename T>
inline typename enable_if<is_same<ObjectConstRef, T>::value, T>::type variantAs(
    const VariantData* _data);

template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data);

template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data);

}  // namespace ARDUINOJSON_NAMESPACE
