// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/ctype.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename A, typename B>
using largest_type = conditional_t<(sizeof(A) > sizeof(B)), A, B>;

enum class NumberType : uint8_t {
  Invalid,
  Float,
  SignedInteger,
  UnsignedInteger,
#if ARDUINOJSON_USE_DOUBLE
  Double,
#endif
};

union NumberValue {
  NumberValue() {}
  NumberValue(float x) : asFloat(x) {}
  NumberValue(JsonInteger x) : asSignedInteger(x) {}
  NumberValue(JsonUInt x) : asUnsignedInteger(x) {}
#if ARDUINOJSON_USE_DOUBLE
  NumberValue(double x) : asDouble(x) {}
#endif

  JsonInteger asSignedInteger;
  JsonUInt asUnsignedInteger;
  float asFloat;
#if ARDUINOJSON_USE_DOUBLE
  double asDouble;
#endif
};

class Number {
  NumberType type_;
  NumberValue value_;

 public:
  Number() : type_(NumberType::Invalid) {}
  Number(float value) : type_(NumberType::Float), value_(value) {}
  Number(JsonInteger value) : type_(NumberType::SignedInteger), value_(value) {}
  Number(JsonUInt value) : type_(NumberType::UnsignedInteger), value_(value) {}
#if ARDUINOJSON_USE_DOUBLE
  Number(double value) : type_(NumberType::Double), value_(value) {}
#endif

  template <typename T>
  T convertTo() const {
    switch (type_) {
      case NumberType::Float:
        return convertNumber<T>(value_.asFloat);
      case NumberType::SignedInteger:
        return convertNumber<T>(value_.asSignedInteger);
      case NumberType::UnsignedInteger:
        return convertNumber<T>(value_.asUnsignedInteger);
#if ARDUINOJSON_USE_DOUBLE
      case NumberType::Double:
        return convertNumber<T>(value_.asDouble);
#endif
      default:
        return T();
    }
  }

  NumberType type() const {
    return type_;
  }

  JsonInteger asSignedInteger() const {
    ARDUINOJSON_ASSERT(type_ == NumberType::SignedInteger);
    return value_.asSignedInteger;
  }

  JsonUInt asUnsignedInteger() const {
    ARDUINOJSON_ASSERT(type_ == NumberType::UnsignedInteger);
    return value_.asUnsignedInteger;
  }

  float asFloat() const {
    ARDUINOJSON_ASSERT(type_ == NumberType::Float);
    return value_.asFloat;
  }

#if ARDUINOJSON_USE_DOUBLE
  double asDouble() const {
    ARDUINOJSON_ASSERT(type_ == NumberType::Double);
    return value_.asDouble;
  }
#endif
};

inline Number parseNumber(const char* s) {
  typedef FloatTraits<JsonFloat> traits;
  typedef largest_type<traits::mantissa_type, JsonUInt> mantissa_t;
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
    return Number(traits::nan());
  }
#endif

#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    return Number(is_negative ? -traits::inf() : traits::inf());
  }
#endif

  if (!isdigit(*s) && *s != '.')
    return Number();

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
        return Number(JsonInteger(~mantissa + 1));
      }
    } else {
      return Number(JsonUInt(mantissa));
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
          return Number(is_negative ? -0.0f : 0.0f);
        else
          return Number(is_negative ? -traits::inf() : traits::inf());
      }
      s++;
    }
    if (negative_exponent)
      exponent = -exponent;
  }
  exponent += exponent_offset;

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0')
    return Number();

#if ARDUINOJSON_USE_DOUBLE
  bool isDouble = exponent < -FloatTraits<float>::exponent_max ||
                  exponent > FloatTraits<float>::exponent_max ||
                  mantissa > FloatTraits<float>::mantissa_max;
  if (isDouble) {
    auto final_result = make_float(double(mantissa), exponent);
    return Number(is_negative ? -final_result : final_result);
  } else
#endif
  {
    auto final_result = make_float(float(mantissa), exponent);
    return Number(is_negative ? -final_result : final_result);
  }
}

template <typename T>
inline T parseNumber(const char* s) {
  return parseNumber(s).convertTo<T>();
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
