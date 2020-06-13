// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantAs.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TVariant>
struct VariantOperators {
  // Returns the default value if the VariantRef is undefined of incompatible
  template <typename T>
  friend T operator|(const TVariant &variant, const T &defaultValue) {
    if (variant.template is<T>())
      return variant.template as<T>();
    else
      return defaultValue;
  }

  // Returns the default value if the VariantRef is undefined of incompatible
  // Special case for string: null is treated as undefined
  friend const char *operator|(const TVariant &variant,
                               const char *defaultValue) {
    const char *value = variant.template as<const char *>();
    return value ? value : defaultValue;
  }

  // value == TVariant
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) == 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) == 0;
  }

  // TVariant == value
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) == 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) == 0;
  }

  // value != TVariant
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) != 0;
  }

  // TVariant != value
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) != 0;
  }

  // value < TVariant
  template <typename T>
  friend bool operator<(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) > 0;
  }
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) > 0;
  }

  // TVariant < value
  template <typename T>
  friend bool operator<(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) < 0;
  }
  template <typename T>
  friend bool operator<(TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) < 0;
  }

  // value <= TVariant
  template <typename T>
  friend bool operator<=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) >= 0;
  }
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) >= 0;
  }

  // TVariant <= value
  template <typename T>
  friend bool operator<=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) <= 0;
  }
  template <typename T>
  friend bool operator<=(TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) <= 0;
  }

  // value > TVariant
  template <typename T>
  friend bool operator>(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) < 0;
  }
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) < 0;
  }

  // TVariant > value
  template <typename T>
  friend bool operator>(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) > 0;
  }
  template <typename T>
  friend bool operator>(TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) > 0;
  }

  // value >= TVariant
  template <typename T>
  friend bool operator>=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) <= 0;
  }
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) <= 0;
  }

  // TVariant >= value
  template <typename T>
  friend bool operator>=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) >= 0;
  }
  template <typename T>
  friend bool operator>=(TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) >= 0;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
