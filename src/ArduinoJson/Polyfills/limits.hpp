// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "type_traits.hpp"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4310)
#endif

namespace ARDUINOJSON_NAMESPACE {

// Differs from standard because we can't use the symbols "min" and "max"
template <typename T, typename Enable = void>
struct numeric_limits;

template <typename T>
struct numeric_limits<T, typename enable_if<is_unsigned<T>::value>::type> {
  static T lowest() {
    return 0;
  }
  static T highest() {
    return T(-1);
  }
};

template <typename T>
struct numeric_limits<
    T, typename enable_if<is_integral<T>::value && is_signed<T>::value>::type> {
  static T lowest() {
    return T(T(1) << (sizeof(T) * 8 - 1));
  }
  static T highest() {
    return T(~lowest());
  }
};

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
