// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
int16_t normalize(T& value) {
  int16_t powersOf10 = 0;

  if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
#if ARDUINOJSON_DOUBLE_IS_64BITS
    if (value >= 1e256) {
      value /= 1e256;
      powersOf10 = int16_t(powersOf10 + 256);
    }
    if (value >= 1e128) {
      value /= 1e128;
      powersOf10 = int16_t(powersOf10 + 128);
    }
    if (value >= 1e64) {
      value /= 1e64;
      powersOf10 = int16_t(powersOf10 + 64);
    }
#endif
    if (value >= 1e32) {
      value /= 1e32;
      powersOf10 = int16_t(powersOf10 + 32);
    }
    if (value >= 1e16) {
      value /= 1e16;
      powersOf10 = int16_t(powersOf10 + 16);
    }
    if (value >= 1e8) {
      value /= 1e8;
      powersOf10 = int16_t(powersOf10 + 8);
    }
    if (value >= 1e4) {
      value /= 1e4;
      powersOf10 = int16_t(powersOf10 + 4);
    }
    if (value >= 1e2) {
      value /= 1e2;
      powersOf10 = int16_t(powersOf10 + 2);
    }
    if (value >= 1e1) {
      value /= 1e1;
      powersOf10 = int16_t(powersOf10 + 1);
    }
  }

  if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
#if ARDUINOJSON_DOUBLE_IS_64BITS
    if (value < 1e-255) {
      value *= 1e256;
      powersOf10 = int16_t(powersOf10 - 256);
    }
    if (value < 1e-127) {
      value *= 1e128;
      powersOf10 = int16_t(powersOf10 - 128);
    }
    if (value < 1e-63) {
      value *= 1e64;
      powersOf10 = int16_t(powersOf10 - 64);
    }
#endif
    if (value < 1e-31) {
      value *= 1e32;
      powersOf10 = int16_t(powersOf10 - 32);
    }
    if (value < 1e-15) {
      value *= 1e16;
      powersOf10 = int16_t(powersOf10 - 16);
    }
    if (value < 1e-7) {
      value *= 1e8;
      powersOf10 = int16_t(powersOf10 - 8);
    }
    if (value < 1e-3) {
      value *= 1e4;
      powersOf10 = int16_t(powersOf10 - 4);
    }
    if (value < 1e-1) {
      value *= 1e2;
      powersOf10 = int16_t(powersOf10 - 2);
    }
    if (value < 1e0) {
      value *= 1e1;
      powersOf10 = int16_t(powersOf10 - 1);
    }
  }

  return powersOf10;
}
}
}
