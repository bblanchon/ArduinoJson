// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantAs.hpp>
#include <ArduinoJson/Variant/VariantTag.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs);  // VariantCompare.cpp

template <typename TVariant>
struct VariantOperators {
  // Returns the default value if the VariantRef is undefined or incompatible
  //
  // int operator|(JsonVariant, int)
  // float operator|(JsonVariant, float)
  // bool operator|(JsonVariant, bool)
  // const char* operator|(JsonVariant, const char*)
  // char* operator|(JsonVariant, const char*)
  template <typename T>
  friend typename enable_if<!IsVariant<T>::value,
                            typename VariantAs<T>::type>::type
  operator|(const TVariant &variant, T defaultValue) {
    if (variant.template is<T>())
      return variant.template as<T>();
    else
      return defaultValue;
  }
  //
  // JsonVariant operator|(JsonVariant, JsonVariant)
  template <typename T>
  friend typename enable_if<IsVariant<T>::value, typename T::variant_type>::type
  operator|(const TVariant &variant, T defaultValue) {
    if (variant)
      return variant;
    else
      return defaultValue;
  }

  // value == TVariant
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator==(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_EQUAL;
  }

  // TVariant == value
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_EQUAL;
  }

  // value != TVariant
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator!=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) != COMPARE_RESULT_EQUAL;
  }

  // TVariant != value
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) != COMPARE_RESULT_EQUAL;
  }

  // value < TVariant
  template <typename T>
  friend bool operator<(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_GREATER;
  }

  // TVariant < value
  template <typename T>
  friend bool operator<(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_LESS;
  }

  // value <= TVariant
  template <typename T>
  friend bool operator<=(T *lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }

  // TVariant <= value
  template <typename T>
  friend bool operator<=(TVariant lhs, T *rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }

  // value > TVariant
  template <typename T>
  friend bool operator>(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_LESS;
  }

  // TVariant > value
  template <typename T>
  friend bool operator>(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_GREATER;
  }

  // value >= TVariant
  template <typename T>
  friend bool operator>=(T *lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }

  // TVariant >= value
  template <typename T>
  friend bool operator>=(TVariant lhs, T *rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
