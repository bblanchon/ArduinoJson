// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Numbers/JsonInteger.hpp>
#include <ArduinoJson/Numbers/StringConversions.hpp>

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
        return is_floating_point<T>::value ? T(NAN) : T();
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
  assert(s != 0);

  auto decimalFloat = stringToDecimal<JsonFloat>(s);
#if ARDUINOJSON_USE_DOUBLE && !ARDUINOJSON_USE_LONG_LONG
  using significant_t = decltype(decimalFloat)::significand_type;
  constexpr auto jsonIntMax = significant_t(1) << (sizeof(JsonInteger) * 8 - 1);
  constexpr auto jsonUintMax = (significant_t(1) << (sizeof(JsonUInt) * 8)) - 1;
#endif

  if (decimalFloat.isError)
    return Number();

  if (decimalFloat.isNaN)
    return NAN;

  if (decimalFloat.isInfinity)
    return decimalFloat.isNegative ? -INFINITY : INFINITY;

  if (decimalFloat.exponent == 0) {
    if (decimalFloat.isNegative) {
#if ARDUINOJSON_USE_DOUBLE && !ARDUINOJSON_USE_LONG_LONG
      // using significant_t = decltype(decimalFloat)::significand_type;
      // constexpr auto jsonIntMax = significant_t(1) << (sizeof(JsonInteger) *
      // 8 - 1); constexpr auto jsonUintMax = significant_t(1) <<
      // (sizeof(JsonUInt) * 8 - 1);
      if (decimalFloat.significand <= jsonIntMax) {
        return Number(JsonInteger(~decimalFloat.significand + 1));
      }
      // integer overflow => store as float
#else
      return Number(JsonInteger(~decimalFloat.significand + 1));
#endif
    } else {
#if ARDUINOJSON_USE_DOUBLE && !ARDUINOJSON_USE_LONG_LONG
      if (decimalFloat.significand <= jsonUintMax) {
        return Number(JsonUInt(decimalFloat.significand));
      }
// integer overflow => store as float
#else
      return Number(JsonUInt(decimalFloat.significand));
#endif
    }
  }

  // TODO: fine-tune the threshold
  auto binaryFloat = decimalToBinaryFloat<JsonFloat>(decimalFloat);
  binaryFloat.normalize();

#if ARDUINOJSON_USE_DOUBLE && \
    (!defined(__SIZEOF_DOUBLE__) || __SIZEOF_DOUBLE__ > 4)
  ARDUINOJSON_ASSERT(sizeof(JsonFloat) == 8);

  bool hasFewDigits = decimalFloat.significand <= 10000000;

  if (hasFewDigits && binaryFloat.exponent <= 64 &&
      binaryFloat.exponent >= -190) {
    Float32 binaryFloat32;
    binaryFloat32.significand = uint32_t(binaryFloat.significand >> 32);
    binaryFloat32.exponent = int16_t(binaryFloat.exponent + 32);
    binaryFloat32.isNegative = binaryFloat.isNegative;
    // TODO: try casting the double to a float to see if it produces smaller
    // code
    return packBinaryFloat<float>(binaryFloat32);
  }
#endif

  return packBinaryFloat<JsonFloat>(binaryFloat);
}

template <typename T>
inline T parseNumber(const char* s) {
  return parseNumber(s).convertTo<T>();
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
