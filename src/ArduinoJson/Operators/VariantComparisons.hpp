// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Variant/VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename T>
struct is_simple_value {
  static const bool value = is_integral<T>::value ||
                            is_floating_point<T>::value ||
                            is_same<T, bool>::value;
};

template <typename TVariant>
class VariantComparisons {
 public:
  // const char* == TVariant
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      T *lhs, TVariant rhs) {
    return adaptString(lhs).equals(rhs.template as<const char *>());
  }

  // std::string == TVariant
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return adaptString(lhs).equals(rhs.template as<const char *>());
  }

  // TVariant == const char*
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator==(
      TVariant lhs, T *rhs) {
    return adaptString(rhs).equals(lhs.template as<const char *>());
  }

  // TVariant == std::string
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return adaptString(rhs).equals(lhs.template as<const char *>());
  }

  // bool/int/float == TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return lhs == rhs.template as<T>();
  }

  // TVariant == bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() == rhs;
  }

  // const char* != TVariant
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      T *lhs, TVariant rhs) {
    return !adaptString(lhs).equals(rhs.template as<const char *>());
  }

  // std::string != TVariant
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return !adaptString(lhs).equals(rhs.template as<const char *>());
  }

  // TVariant != const char*
  template <typename T>
  friend typename enable_if<IsString<T *>::value, bool>::type operator!=(
      TVariant lhs, T *rhs) {
    return !adaptString(rhs).equals(lhs.template as<const char *>());
  }

  // TVariant != std::string
  template <typename T>
  friend typename enable_if<IsString<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return !adaptString(rhs).equals(lhs.template as<const char *>());
  }

  // bool/int/float != TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return lhs != rhs.template as<T>();
  }

  // TVariant != bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() != rhs;
  }

  // bool/int/float < TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      const T &lhs, TVariant rhs) {
    return lhs < rhs.template as<T>();
  }

  // TVariant < bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() < rhs;
  }

  // bool/int/float <= TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      const T &lhs, TVariant rhs) {
    return lhs <= rhs.template as<T>();
  }

  // TVariant <= bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() <= rhs;
  }

  // bool/int/float > TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      const T &lhs, TVariant rhs) {
    return lhs > rhs.template as<T>();
  }

  // TVariant > bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() > rhs;
  }

  // bool/int/float >= TVariant
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      const T &lhs, TVariant rhs) {
    return lhs >= rhs.template as<T>();
  }

  // TVariant >= bool/int/float
  template <typename T>
  friend typename enable_if<is_simple_value<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return lhs.template as<T>() >= rhs;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
