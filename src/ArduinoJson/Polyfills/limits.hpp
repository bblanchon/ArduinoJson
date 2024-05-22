// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "type_traits.hpp"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4310)
#endif

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Differs from standard because we can't use the symbols "min" and "max"
template <typename T, typename Enable = void>
struct numeric_limits;

template <typename T>
struct numeric_limits<T, enable_if_t<is_unsigned<T>::value>> {
  static constexpr T lowest() {
    return 0;
  }
  static constexpr T highest() {
    return T(-1);
  }
};

template <typename T>
struct numeric_limits<
    T, enable_if_t<is_integral<T>::value && is_signed<T>::value>> {
  static constexpr T lowest() {
    return T(T(1) << (sizeof(T) * 8 - 1));
  }
  static constexpr T highest() {
    return T(~lowest());
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
