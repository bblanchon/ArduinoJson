// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/ctype.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TFloat, typename TUInt>
struct ParsedNumber {
  ParsedNumber() : _type(VALUE_IS_NULL) {}

  void setInteger(TUInt value, bool is_negative) {
    uintValue = value;
    _type = uint8_t(is_negative ? VALUE_IS_NEGATIVE_INTEGER
                                : VALUE_IS_POSITIVE_INTEGER);
  }

  void setFloat(TFloat value) {
    floatValue = value;
    _type = VALUE_IS_FLOAT;
  }

  template <typename T>
  T as() const {
    switch (_type) {
      case VALUE_IS_NEGATIVE_INTEGER:
        return convertNegativeInteger<T>(uintValue);
      case VALUE_IS_POSITIVE_INTEGER:
        return convertPositiveInteger<T>(uintValue);
      case VALUE_IS_FLOAT:
        return convertFloat<T>(floatValue);
      default:
        return 0;
    }
  }

  uint8_t type() const {
    return _type;
  }

  union {
    TUInt uintValue;
    TFloat floatValue;
  };
  uint8_t _type;
};  // namespace ARDUINOJSON_NAMESPACE

template <typename A, typename B>
struct choose_largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};

template <typename TFloat, typename TUInt>
inline void parseNumber(const char* s, ParsedNumber<TFloat, TUInt>& result) {
  typedef FloatTraits<TFloat> traits;
  typedef typename choose_largest<typename traits::mantissa_type, TUInt>::type
      mantissa_t;
  typedef typename traits::exponent_type exponent_t;

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
    return;
  }

#endif

#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    result.setFloat(is_negative ? -traits::inf() : traits::inf());
    return;
  }
#endif

  if (!isdigit(*s) && *s != '.')
    return;

  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  const mantissa_t maxUint = TUInt(-1);

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
    result.setInteger(TUInt(mantissa), is_negative);
    return;
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
        return;
      }
      s++;
    }
    if (negative_exponent)
      exponent = -exponent;
  }
  exponent += exponent_offset;

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0')
    return;

  TFloat final_result =
      traits::make_float(static_cast<TFloat>(mantissa), exponent);

  result.setFloat(is_negative ? -final_result : final_result);
}
}  // namespace ARDUINOJSON_NAMESPACE
