// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}

// bool is<char>() const;
// bool is<signed char>() const;
// bool is<signed short>() const;
// bool is<signed int>() const;
// bool is<signed long>() const;
// bool is<unsigned char>() const;
// bool is<unsigned short>() const;
// bool is<unsigned int>() const;
// bool is<unsigned long>() const;
template <typename T>
NO_INLINE typename enable_if<is_integral<T>::value && !is_same<bool, T>::value,
                             bool>::type
variantIs(const VariantData *var) {
  return var && var->isInteger<T>();
}

// bool is<double>() const;
// bool is<float>() const;
template <typename T>
NO_INLINE typename enable_if<is_floating_point<T>::value, bool>::type variantIs(
    const VariantData *var) {
  return var && var->isFloat();
}

// bool is<bool>() const
template <typename T>
NO_INLINE typename enable_if<is_same<T, bool>::value, bool>::type variantIs(
    const VariantData *var) {
  return var && var->isBoolean();
}

// bool is<const char*>() const;
// bool is<char*>() const;
// bool is<std::string>() const;
// bool is<String>() const;
template <typename T>
NO_INLINE typename enable_if<is_same<T, const char *>::value ||
                                 is_same<T, char *>::value ||
                                 IsWriteableString<T>::value,
                             bool>::type
variantIs(const VariantData *var) {
  return var && var->isString();
}

// bool is<ArrayConstRef> const;
// bool is<const ArrayConstRef> const;
template <typename T>
NO_INLINE typename enable_if<
    is_same<typename remove_const<T>::type, ArrayConstRef>::value, bool>::type
variantIs(const VariantData *var) {
  return var && var->isArray();
}

// bool is<ObjectConstRef> const;
// bool is<const ObjectConstRef> const;
template <typename T>
NO_INLINE typename enable_if<
    is_same<typename remove_const<T>::type, ObjectConstRef>::value, bool>::type
variantIs(const VariantData *var) {
  return var && var->isObject();
}

// bool is<VariantConstRef> const;
// bool is<const VariantConstRef> const;
template <typename T>
NO_INLINE typename enable_if<
    is_same<typename remove_const<T>::type, VariantConstRef>::value, bool>::type
variantIs(const VariantData *var) {
  return !!var;
}
#if ARDUINOJSON_HAS_NULLPTR

// bool is<nullptr_t> const;
template <typename T>
NO_INLINE typename enable_if<is_same<T, decltype(nullptr)>::value, bool>::type
variantIs(const VariantData *var) {
  return variantIsNull(var);
}
#endif
// bool is<enum>() const;
template <typename T>
typename enable_if<is_enum<T>::value, bool>::type variantIs(
    const VariantData *var) {
  return variantIs<int>(var);
}

// bool is<ArrayRef> const;
// bool is<const ArrayRef> const;
template <typename T>
NO_INLINE
    typename enable_if<is_same<typename remove_const<T>::type, ArrayRef>::value,
                       bool>::type
    variantIs(VariantData *var) {
  return var && var->isArray();
}

// bool is<ObjectRef> const;
// bool is<const ObjectRef> const;
template <typename T>
NO_INLINE typename enable_if<
    is_same<typename remove_const<T>::type, ObjectRef>::value, bool>::type
variantIs(VariantData *var) {
  return var && var->isObject();
}

// bool is<VariantRef> const;
// bool is<const VariantRef> const;
template <typename T>
NO_INLINE typename enable_if<
    is_same<typename remove_const<T>::type, VariantRef>::value, bool>::type
variantIs(VariantData *var) {
  return !!var;
}

// bool is<ArrayRef> const;
// bool is<const ArrayRef> const;
// bool is<ObjectRef> const;
// bool is<const ObjectRef> const;
// bool is<VariantRef> const;
// bool is<const VariantRef> const;
template <typename T>
typename enable_if<
    is_same<typename remove_const<T>::type, ArrayRef>::value ||
        is_same<typename remove_const<T>::type, ObjectRef>::value ||
        is_same<typename remove_const<T>::type, VariantRef>::value,
    bool>::type
variantIs(const VariantData *) {
  return false;
}

}  // namespace ARDUINOJSON_NAMESPACE
