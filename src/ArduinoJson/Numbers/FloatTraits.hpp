// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Polyfills/alias_cast.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/pgmspace_generic.hpp>
#include <ArduinoJson/Polyfills/preprocessor.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T, size_t = sizeof(T)>
struct FloatTraits {};

template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef uint64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (mantissa_type(1) << mantissa_bits) - 1;

  typedef int16_t exponent_type;
  static const exponent_type exponent_max = 308;

  static pgm_ptr<T> positiveBinaryPowersOfTen() {
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(  //
        uint64_t, factors,
        {
            0x4024000000000000,  // 1e1
            0x4059000000000000,  // 1e2
            0x40C3880000000000,  // 1e4
            0x4197D78400000000,  // 1e8
            0x4341C37937E08000,  // 1e16
            0x4693B8B5B5056E17,  // 1e32
            0x4D384F03E93FF9F5,  // 1e64
            0x5A827748F9301D32,  // 1e128
            0x75154FDD7F73BF3C,  // 1e256
        });
    return pgm_ptr<T>(reinterpret_cast<const T*>(factors));
  }

  static pgm_ptr<T> negativeBinaryPowersOfTen() {
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(  //
        uint64_t, factors,
        {
            0x3FB999999999999A,  // 1e-1
            0x3F847AE147AE147B,  // 1e-2
            0x3F1A36E2EB1C432D,  // 1e-4
            0x3E45798EE2308C3A,  // 1e-8
            0x3C9CD2B297D889BC,  // 1e-16
            0x3949F623D5A8A733,  // 1e-32
            0x32A50FFD44F4A73D,  // 1e-64
            0x255BBA08CF8C979D,  // 1e-128
            0x0AC8062864AC6F43   // 1e-256
        });
    return pgm_ptr<T>(reinterpret_cast<const T*>(factors));
  }

  static T nan() {
    return forge(0x7ff8000000000000);
  }

  static T inf() {
    return forge(0x7ff0000000000000);
  }

  static T highest() {
    return forge(0x7FEFFFFFFFFFFFFF);
  }

  template <typename TOut>  // int64_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_signed<TOut>::value &&
                      sizeof(TOut) == 8,
                  signed>* = 0) {
    return forge(0x43DFFFFFFFFFFFFF);  //  9.2233720368547748e+18
  }

  template <typename TOut>  // uint64_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_unsigned<TOut>::value &&
                      sizeof(TOut) == 8,
                  unsigned>* = 0) {
    return forge(0x43EFFFFFFFFFFFFF);  //  1.8446744073709549568e+19
  }

  static T lowest() {
    return forge(0xFFEFFFFFFFFFFFFF);
  }

  // constructs a double floating point values from its binary representation
  // we use this function to workaround platforms with single precision literals
  // (for example, when -fsingle-precision-constant is passed to GCC)
  static T forge(uint64_t bits) {
    return alias_cast<T>(bits);
  }
};

template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef uint32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (mantissa_type(1) << mantissa_bits) - 1;

  typedef int8_t exponent_type;
  static const exponent_type exponent_max = 38;

  static pgm_ptr<T> positiveBinaryPowersOfTen() {
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(uint32_t, factors,
                                     {
                                         0x41200000,  // 1e1f
                                         0x42c80000,  // 1e2f
                                         0x461c4000,  // 1e4f
                                         0x4cbebc20,  // 1e8f
                                         0x5a0e1bca,  // 1e16f
                                         0x749dc5ae   // 1e32f
                                     });
    return pgm_ptr<T>(reinterpret_cast<const T*>(factors));
  }

  static pgm_ptr<T> negativeBinaryPowersOfTen() {
    ARDUINOJSON_DEFINE_PROGMEM_ARRAY(uint32_t, factors,
                                     {
                                         0x3dcccccd,  // 1e-1f
                                         0x3c23d70a,  // 1e-2f
                                         0x38d1b717,  // 1e-4f
                                         0x322bcc77,  // 1e-8f
                                         0x24e69595,  // 1e-16f
                                         0x0a4fb11f   // 1e-32f
                                     });
    return pgm_ptr<T>(reinterpret_cast<const T*>(factors));
  }

  static T forge(uint32_t bits) {
    return alias_cast<T>(bits);
  }

  static T nan() {
    return forge(0x7fc00000);
  }

  static T inf() {
    return forge(0x7f800000);
  }

  static T highest() {
    return forge(0x7f7fffff);
  }

  template <typename TOut>  // int32_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_signed<TOut>::value &&
                      sizeof(TOut) == 4,
                  signed>* = 0) {
    return forge(0x4EFFFFFF);  // 2.14748352E9
  }

  template <typename TOut>  // uint32_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_unsigned<TOut>::value &&
                      sizeof(TOut) == 4,
                  unsigned>* = 0) {
    return forge(0x4F7FFFFF);  // 4.29496704E9
  }

  template <typename TOut>  // int64_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_signed<TOut>::value &&
                      sizeof(TOut) == 8,
                  signed>* = 0) {
    return forge(0x5EFFFFFF);  // 9.22337148709896192E18
  }

  template <typename TOut>  // uint64_t
  static T highest_for(
      enable_if_t<is_integral<TOut>::value && is_unsigned<TOut>::value &&
                      sizeof(TOut) == 8,
                  unsigned>* = 0) {
    return forge(0x5F7FFFFF);  // 1.844674297419792384E19
  }

  static T lowest() {
    return forge(0xFf7fffff);
  }
};

template <typename TFloat, typename TExponent>
inline TFloat make_float(TFloat m, TExponent e) {
  using traits = FloatTraits<TFloat>;

  auto powersOfTen = e > 0 ? traits::positiveBinaryPowersOfTen()
                           : traits::negativeBinaryPowersOfTen();
  if (e <= 0)
    e = TExponent(-e);

  for (uint8_t index = 0; e != 0; index++) {
    if (e & 1)
      m *= powersOfTen[index];
    e >>= 1;
  }
  return m;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
