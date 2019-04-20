// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/assert.hpp"
#include "../Polyfills/ctype.hpp"
#include "../Polyfills/math.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Variant/VariantContent.hpp"
#include "FloatTraits.hpp"
#include "convertNumber.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TFloat, typename TUInt>
struct ParsedNumber {
  ParsedNumber() : uintValue(0), floatValue(0), _type(VALUE_IS_NULL) {}

  ParsedNumber(TUInt value, bool is_negative)
      : uintValue(value),
        floatValue(TFloat(value)),
        _type(uint8_t(is_negative ? VALUE_IS_NEGATIVE_INTEGER
                                  : VALUE_IS_POSITIVE_INTEGER)) {}
  ParsedNumber(TFloat value) : floatValue(value), _type(VALUE_IS_FLOAT) {}

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

  TUInt uintValue;
  TFloat floatValue;
  uint8_t _type;
};

template <typename A, typename B>
struct choose_largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};

template <typename TFloat, typename TUInt>
inline ParsedNumber<TFloat, TUInt> parseNumber(const char *s) {
  typedef FloatTraits<TFloat> traits;
  typedef typename choose_largest<typename traits::mantissa_type, TUInt>::type
      mantissa_t;
  typedef typename traits::exponent_type exponent_t;
  typedef ParsedNumber<TFloat, TUInt> return_type;

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

  if (*s == 'n' || *s == 'N') return traits::nan();
  if (*s == 'i' || *s == 'I')
    return is_negative ? -traits::inf() : traits::inf();
  if (!isdigit(*s) && *s != '.') return return_type();

  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  const mantissa_t maxUint = TUInt(-1);

  while (isdigit(*s)) {
    uint8_t digit = uint8_t(*s - '0');
    if (mantissa > maxUint / 10) break;
    mantissa *= 10;
    if (mantissa > maxUint - digit) break;
    mantissa += digit;
    s++;
  }

  if (*s == '\0') return return_type(TUInt(mantissa), is_negative);

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
          return is_negative ? -0.0f : 0.0f;
        else
          return is_negative ? -traits::inf() : traits::inf();
      }
      s++;
    }
    if (negative_exponent) exponent = -exponent;
  }
  exponent += exponent_offset;

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0') return return_type();

  TFloat result = traits::make_float(static_cast<TFloat>(mantissa), exponent);

  return is_negative ? -result : result;
}
}  // namespace ARDUINOJSON_NAMESPACE
