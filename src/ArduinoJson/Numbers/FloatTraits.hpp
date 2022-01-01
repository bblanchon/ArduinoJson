// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Polyfills/alias_cast.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/preprocessor.hpp>
#include <ArduinoJson/Polyfills/static_array.hpp>

namespace ARDUINOJSON_NAMESPACE {

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

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = TExponent(-e);
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }

  static T positiveBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x40240000, 0x00000000,  // 1e1
            0x40590000, 0x00000000,  // 1e2
            0x40C38800, 0x00000000,  // 1e4
            0x4197D784, 0x00000000,  // 1e8
            0x4341C379, 0x37E08000,  // 1e16
            0x4693B8B5, 0xB5056E17,  // 1e32
            0x4D384F03, 0xE93FF9F5,  // 1e64
            0x5A827748, 0xF9301D32,  // 1e128
            0x75154FDD, 0x7F73BF3C   // 1e256
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }

  static T negativeBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x3FB99999, 0x9999999A,  // 1e-1
            0x3F847AE1, 0x47AE147B,  // 1e-2
            0x3F1A36E2, 0xEB1C432D,  // 1e-4
            0x3E45798E, 0xE2308C3A,  // 1e-8
            0x3C9CD2B2, 0x97D889BC,  // 1e-16
            0x3949F623, 0xD5A8A733,  // 1e-32
            0x32A50FFD, 0x44F4A73D,  // 1e-64
            0x255BBA08, 0xCF8C979D,  // 1e-128
            0x0AC80628, 0x64AC6F43   // 1e-256
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }

  static T negativeBinaryPowerOfTenPlusOne(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x3FF00000, 0x00000000,  // 1e0
            0x3FB99999, 0x9999999A,  // 1e-1
            0x3F50624D, 0xD2F1A9FC,  // 1e-3
            0x3E7AD7F2, 0x9ABCAF48,  // 1e-7
            0x3CD203AF, 0x9EE75616,  // 1e-15
            0x398039D6, 0x65896880,  // 1e-31
            0x32DA53FC, 0x9631D10D,  // 1e-63
            0x25915445, 0x81B7DEC2,  // 1e-127
            0x0AFE07B2, 0x7DD78B14   // 1e-255
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }

  static T nan() {
    return forge(0x7ff80000, 0x00000000);
  }

  static T inf() {
    return forge(0x7ff00000, 0x00000000);
  }

  static T highest() {
    return forge(0x7FEFFFFF, 0xFFFFFFFF);
  }

  static T lowest() {
    return forge(0xFFEFFFFF, 0xFFFFFFFF);
  }

  // constructs a double floating point values from its binary representation
  // we use this function to workaround platforms with single precision literals
  // (for example, when -fsingle-precision-constant is passed to GCC)
  static T forge(uint32_t msb, uint32_t lsb) {
    return alias_cast<T>((uint64_t(msb) << 32) | lsb);
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

  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = -e;
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }

  static T positiveBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(uint32_t, factors,
                                    ARDUINOJSON_EXPAND6({
                                        0x41200000,  // 1e1f
                                        0x42c80000,  // 1e2f
                                        0x461c4000,  // 1e4f
                                        0x4cbebc20,  // 1e8f
                                        0x5a0e1bca,  // 1e16f
                                        0x749dc5ae   // 1e32f
                                    }));
    return forge(ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, index));
  }

  static T negativeBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(uint32_t, factors,
                                    ARDUINOJSON_EXPAND6({
                                        0x3dcccccd,  // 1e-1f
                                        0x3c23d70a,  // 1e-2f
                                        0x38d1b717,  // 1e-4f
                                        0x322bcc77,  // 1e-8f
                                        0x24e69595,  // 1e-16f
                                        0x0a4fb11f   // 1e-32f
                                    }));
    return forge(ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, index));
  }

  static T negativeBinaryPowerOfTenPlusOne(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(uint32_t, factors,
                                    ARDUINOJSON_EXPAND6({
                                        0x3f800000,  // 1e0f
                                        0x3dcccccd,  // 1e-1f
                                        0x3a83126f,  // 1e-3f
                                        0x33d6bf95,  // 1e-7f
                                        0x26901d7d,  // 1e-15f
                                        0x0c01ceb3   // 1e-31f
                                    }));
    return forge(ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, index));
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

  static T lowest() {
    return forge(0xFf7fffff);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
