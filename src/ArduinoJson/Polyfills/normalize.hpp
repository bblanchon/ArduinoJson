// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"
#include "../TypeTraits/FloatTraits.hpp"

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
int16_t normalize(T& value) {
  using namespace TypeTraits;
  int16_t powersOf10 = 0;

  int8_t index = sizeof(T) == 8 ? 8 : 5;
  int bit = 1 << index;

  if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
    for (; index >= 0; index--) {
      if (value >= FloatTraits<T>::positiveBinaryPowerOfTen(index)) {
        value *= FloatTraits<T>::negativeBinaryPowerOfTen(index);
        powersOf10 = int16_t(powersOf10 + bit);
      }
      bit >>= 1;
    }
  }

  if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
    for (; index >= 0; index--) {
      if (value < FloatTraits<T>::negativeBinaryPowerOfTenPlusOne(index)) {
        value *= FloatTraits<T>::positiveBinaryPowerOfTen(index);
        powersOf10 = int16_t(powersOf10 - bit);
      }
      bit >>= 1;
    }
  }

  return powersOf10;
}
}
}
