// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../../Configuration.hpp"
#include "is_same.hpp"

namespace ARDUINOJSON_NAMESPACE {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_integral {
  static const bool value =
      is_same<T, signed char>::value || is_same<T, unsigned char>::value ||
      is_same<T, signed short>::value || is_same<T, unsigned short>::value ||
      is_same<T, signed int>::value || is_same<T, unsigned int>::value ||
      is_same<T, signed long>::value || is_same<T, unsigned long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
      is_same<T, signed long long>::value ||
      is_same<T, unsigned long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
      is_same<T, signed __int64>::value ||
      is_same<T, unsigned __int64>::value ||
#endif
      is_same<T, char>::value;

  // CAUTION: differs from std::is_integral as it doesn't include bool
};

template <typename T>
struct is_integral<const T> : is_integral<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
