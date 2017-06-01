// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <stdint.h>
#include <stdlib.h>  // for size_t
#include "../Configuration.hpp"
#include "../Polyfills/math.hpp"

namespace ArduinoJson {
namespace TypeTraits {

template <typename T, size_t = sizeof(T)>
struct FloatTraits {};

#if ARDUINOJSON_DOUBLE_IS_64BITS
template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef int64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int16_t exponent_type;
  static const exponent_type exponent_max = 308;

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e >= 0)
      return m * (e & 1 ? 1e1 : 1) * (e & 2 ? 1e2 : 1) * (e & 4 ? 1e4 : 1) *
             (e & 8 ? 1e8 : 1) * (e & 16 ? 1e16 : 1) * (e & 32 ? 1e32 : 1) *
             (e & 64 ? 1e64 : 1) * (e & 128 ? 1e128 : 1) *
             (e & 256 ? 1e256 : 1);
    e = TExponent(-e);
    return m * (e & 1 ? 1e-1 : 1) * (e & 2 ? 1e-2 : 1) * (e & 4 ? 1e-4 : 1) *
           (e & 8 ? 1e-8 : 1) * (e & 16 ? 1e-16 : 1) * (e & 32 ? 1e-32 : 1) *
           (e & 64 ? 1e-64 : 1) * (e & 128 ? 1e-128 : 1) *
           (e & 256 ? 1e-256 : 1);
  }

  static T nan() {
    uint64_t x = uint64_t(0x7ff8) << 48;
    return *reinterpret_cast<T*>(&x);
  }

  static T inf() {
    uint64_t x = uint64_t(0x7ff0) << 48;
    return *reinterpret_cast<T*>(&x);
  }
};
#endif

template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef int32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int8_t exponent_type;
  static const exponent_type exponent_max = 38;

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0)
      return m * (e & 1 ? 1e1f : 1) * (e & 2 ? 1e2f : 1) * (e & 4 ? 1e4f : 1) *
             (e & 8 ? 1e8f : 1) * (e & 16 ? 1e16f : 1) * (e & 32 ? 1e32f : 1);
    e = -e;
    return m * (e & 1 ? 1e-1f : 1) * (e & 2 ? 1e-2f : 1) * (e & 4 ? 1e-4f : 1) *
           (e & 8 ? 1e-8f : 1) * (e & 16 ? 1e-16f : 1) * (e & 32 ? 1e-32f : 1);
  }

  static T nan() {
    uint32_t x = 0x7fc00000;
    return *reinterpret_cast<T*>(&x);
  }

  static T inf() {
    uint32_t x = 0x7f800000;
    return *reinterpret_cast<T*>(&x);
  }
};
}
}
