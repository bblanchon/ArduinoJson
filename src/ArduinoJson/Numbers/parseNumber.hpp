// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/ctype.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/Converter.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename A, typename B>
struct choose_largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};

inline bool parseNumber(const char* s, VariantData& result) {
  typedef FloatTraits<JsonFloat> traits;
  typedef choose_largest<traits::mantissa_type, JsonUInt>::type mantissa_t;
  typedef traits::exponent_type exponent_t;

  ARDUINOJSON_ASSERT(s != 0);

  bool is_negative = false;
  switch (*s) {
    case '-':
      is_negative = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }

#if ARDUINOJSON_ENABLE_NAN
  if (*s == 'n' || *s == 'N') {
    result.setFloat(traits::nan());
    return true;
  }
#endif

#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    result.setFloat(is_negative ? -traits::inf() : traits::inf());
    return true;
  }
#endif

  if (!isdigit(*s) && *s != '.')
    return false;

  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  const mantissa_t maxUint = JsonUInt(-1);

  while (isdigit(*s)) {
    uint8_t digit = uint8_t(*s - '0');
    if (mantissa > maxUint / 10)
      break;
    mantissa *= 10;
    if (mantissa > maxUint - digit)
      break;
    mantissa += digit;
    s++;
  }

  if (*s == '\0') {
    if (is_negative) {
      const mantissa_t sintMantissaMax = mantissa_t(1)
                                         << (sizeof(JsonInteger) * 8 - 1);
      if (mantissa <= sintMantissaMax) {
        result.setInteger(JsonInteger(~mantissa + 1));
        return true;
      }
    } else {
      result.setInteger(JsonUInt(mantissa));
      return true;
    }
  }

  // avoid mantissa overflow
  while (mantissa > traits::mantissa_max) {
    mantissa /= 10;
    exponent_offset++;
  }

  // remaing digits can't fit in the mantissa
  while (isdigit(*s)) {
    exponent_offset++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + uint8_t(*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }

  int exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while (isdigit(*s)) {
      exponent = exponent * 10 + (*s - '0');
      if (exponent + exponent_offset > traits::exponent_max) {
        if (negative_exponent)
          result.setFloat(is_negative ? -0.0f : 0.0f);
        else
          result.setFloat(is_negative ? -traits::inf() : traits::inf());
        return true;
      }
      s++;
    }
    if (negative_exponent)
      exponent = -exponent;
  }
  exponent += exponent_offset;

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0')
    return false;

  JsonFloat final_result =
      make_float(static_cast<JsonFloat>(mantissa), exponent);

  result.setFloat(is_negative ? -final_result : final_result);
  return true;
}

template <typename T>
inline T parseNumber(const char* s) {
  VariantData value;
  value.init();  // VariantData is a POD, so it has no constructor
  parseNumber(s, value);
  return Converter<T>::fromJson(JsonVariantConst(&value));
}
ARDUINOJSON_END_PRIVATE_NAMESPACE
