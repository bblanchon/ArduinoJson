// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Polyfills/math.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct FloatParts {
  uint32_t integral;
  uint32_t decimal;
  int16_t exponent;
  int8_t decimalPlaces;
};

template <typename TFloat>
inline int16_t normalize(TFloat& value) {
  using traits = FloatTraits<TFloat>;
  int16_t powersOf10 = 0;

  int8_t index = sizeof(TFloat) == 8 ? 8 : 5;
  int bit = 1 << index;

  if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
    for (; index >= 0; index--) {
      if (value >= traits::positiveBinaryPowersOfTen()[index]) {
        value *= traits::negativeBinaryPowersOfTen()[index];
        powersOf10 = int16_t(powersOf10 + bit);
      }
      bit >>= 1;
    }
  }

  if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
    for (; index >= 0; index--) {
      if (value < traits::negativeBinaryPowersOfTen()[index] * 10) {
        value *= traits::positiveBinaryPowersOfTen()[index];
        powersOf10 = int16_t(powersOf10 - bit);
      }
      bit >>= 1;
    }
  }

  return powersOf10;
}

constexpr uint32_t pow10(int exponent) {
  return (exponent == 0) ? 1 : 10 * pow10(exponent - 1);
}

inline FloatParts decomposeFloat(JsonFloat value, int8_t decimalPlaces) {
  uint32_t maxDecimalPart = pow10(decimalPlaces);

  int16_t exponent = normalize(value);

  uint32_t integral = uint32_t(value);
  // reduce number of decimal places by the number of integral places
  for (uint32_t tmp = integral; tmp >= 10; tmp /= 10) {
    maxDecimalPart /= 10;
    decimalPlaces--;
  }

  JsonFloat remainder =
      (value - JsonFloat(integral)) * JsonFloat(maxDecimalPart);

  uint32_t decimal = uint32_t(remainder);
  remainder = remainder - JsonFloat(decimal);

  // rounding:
  // increment by 1 if remainder >= 0.5
  decimal += uint32_t(remainder * 2);
  if (decimal >= maxDecimalPart) {
    decimal = 0;
    integral++;
    if (exponent && integral >= 10) {
      exponent++;
      integral = 1;
    }
  }

  // remove trailing zeros
  while (decimal % 10 == 0 && decimalPlaces > 0) {
    decimal /= 10;
    decimalPlaces--;
  }

  return {integral, decimal, exponent, decimalPlaces};
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
