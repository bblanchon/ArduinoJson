// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/bit.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

#include <math.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TSignificand, typename TExponent, size_t N>
struct Cache {
  static constexpr size_t size = N;
  TSignificand positiveSignificands[N];
  TSignificand negativeSignificands[N];
  TExponent positiveExponents[N];
  TExponent negativeExponents[N];
};

using Cache64 = Cache<uint64_t, int16_t, 9>;
using Cache32 = Cache<uint32_t, int8_t, 6>;

inline const Cache32& getCache(float) {
  static const Cache32 cache = {
      // Binary significand of positive powers of ten
      {
          0x9dc5ada8,  // 1e32
          0x8e1bc9bf,  // 1e16
          0xbebc2000,  // 1e8
          0x9c400000,  // 1e4
          0xc8000000,  // 1e2
          0xa0000000,  // 1e1
      },
      // Binary significand of negative powers of ten
      {
          0xcfb11ead,  // 1e-32
          0xe69594be,  // 1e-16
          0xabcc7711,  // 1e-8
          0xd1b71758,  // 1e-4
          0xa3d70a3d,  // 1e-2
          0xcccccccc,  // 1e-1
      },
      // Binary exponents of positive powers of ten
      {
          107,  // 1e32  (lowest possible exponent is -180)
          54,   // 1e16
          27,   // 1e8
          14,   // 1e4
          7,    // 1e2
          4,    // 1e1
      },
      // Binary exponents of negative powers of ten
      {
          -106,  // 1e-32 (highest possible exponent is 96)
          -53,   // 1e-16
          -26,   // 1e-8
          -13,   // 1e-4
          -6,    // 1e-2
          -3,    // 1e-1
      },
      // HACK: exponents include a +32 offset to simplify the muliplication
  };
  return cache;
}

// TODO: move this to a class
inline const Cache64& getCache(double) {
  static const Cache64 cache = {
      // Binary significand of positive powers of ten
      {
          0xaa7eebfb9df9de8d,  // 1e256
          0x93ba47c980e98cdf,  // 1e128
          0xc2781f49ffcfa6d5,  // 1e64
          0x9dc5ada82b70b59d,  // 1e32
          0x8e1bc9bf04000000,  // 1e16
          0xbebc200000000000,  // 1e8
          0x9c40000000000000,  // 1e4
          0xc800000000000000,  // 1e2
          0xa000000000000000,  // 1e1
      },
      // Binary significand of negative powers of ten
      {
          0xc0314325637a1939,  // 1e-256
          0xddd0467c64bce4a0,  // 1e-128
          0xa87fea27a539e9a5,  // 1e-64
          0xcfb11ead453994ba,  // 1e-32
          0xe69594bec44de15b,  // 1e-16
          0xabcc77118461cefc,  // 1e-8
          0xd1b71758e219652b,  // 1e-4
          0xa3d70a3d70a3d70a,  // 1e-2
          0xcccccccccccccccc,  // 1e-1
      },
      // Binary exponents of positive powers of ten
      {
          851,  // 1e256 (lowest possible exponent is -1137)
          426,  // 1e128
          213,  // 1e64
          107,  // 1e32
          54,   // 1e16
          27,   // 1e8
          14,   // 1e4
          7,    // 1e2
          4,    // 1e1
      },
      // Binary exponents of negative powers of ten
      {
          -850,  // 1e-256 (highest possible exponent is 960)
          -425,  // 1e-128
          -212,  // 1e-64
          -106,  // 1e-32
          -53,   // 1e-16
          -26,   // 1e-8
          -13,   // 1e-4
          -6,    // 1e-2
          -3,    // 1e-1
      },
      // HACK: exponents include a +64 offset to simplify the muliplication
  };
  return cache;
}

template <typename T>
static T multiplyHigh(T a, T b) {
  // TODO: see https://stackoverflow.com/a/50958815/1164966

  constexpr int half = sizeof(T) * 4;
  const T mask = (T(1) << half) - 1;

  T a_lo = a & mask;
  T a_hi = a >> half;
  T b_lo = b & mask;
  T b_hi = b >> half;

  T ac = a_hi * b_hi;
  T bc = a_lo * b_hi;
  T ad = a_hi * b_lo;
  T bd = a_lo * b_lo;

  T tmp = (bd >> half) + (ad & mask) + (bc & mask);
  tmp += T(1) << (half - 1);  // round

  return ac + (ad >> half) + (bc >> half) + (tmp >> half);
}

template <typename TSignificand, typename TExponent>
struct FloatBase {
  using significand_type = TSignificand;
  using exponent_type = TExponent;

  significand_type significand = 0;
  exponent_type exponent = 0;
  bool isNegative = false;
  bool isNaN = false;
  bool isInfinity = false;

  // Followings are only used in stringToDecimal()
  // TODO: use a dedicated type?
  bool isError = false;

  bool normalized() {
    constexpr auto msb = TSignificand(1) << (sizeof(TSignificand) * 8 - 1);
    return significand == 0 || (significand & msb) != 0;
  }

  void normalize() {
    if (!significand)
      return;
    constexpr auto msb = TSignificand(1) << (sizeof(TSignificand) * 8 - 1);
    while ((significand & msb) == 0) {
      significand <<= 1;
      exponent--;
    }
    assert(normalized());
  }
};

using Float64 = FloatBase<uint64_t, int16_t>;
using Float32 = FloatBase<uint32_t, int16_t>;

template <typename T>
using Float = typename std::conditional<sizeof(T) == 8, Float64, Float32>::type;

template <typename TFloat>
const char* decimalToString(const TFloat& value, bool useScientificNotation,
                            char* buf) {
  if (value.isNaN)
    return "NaN";

  if (value.isInfinity)
    return value.isNegative ? "-Infinity" : "Infinity";

  if (value.significand == 0)
    return value.isNegative ? "-0" : "0";

  std::string result;

  int index = sizeof(buf) - 1;
  buf[index--] = '\0';

  // position of the decimal point starting from the end
  auto m = value.significand;

  if (useScientificNotation) {
    auto e = value.exponent;

    // remove trailing zeros
    while (m % 10 == 0) {
      m /= 10;
      e++;
    }

    for (;;) {
      assert(index >= 0);
      buf[index--] = char('0' + m % 10);
      m /= 10;
      if (m == 0)
        break;
      e++;
      if (m < 10)
        buf[index--] = '.';
    }

    result.append(&buf[index + 1]);

    if (e != 0) {
      sprintf(buf, "e%d", e);
      result.append(buf);
    }
  } else {
    auto pointPosition = -value.exponent;

    // remove trailing zeros after the decimal point
    while (m % 10 == 0 && pointPosition > 0) {
      m /= 10;
      pointPosition--;
    }

    // print trailing zeros
    while (pointPosition < 0) {
      buf[index--] = '0';
      pointPosition++;
    }

    while (m > 0 || pointPosition >= 0) {
      assert(index >= 0);
      buf[index--] = char('0' + m % 10);
      m /= 10;
      if (pointPosition == 1)
        buf[index--] = '.';
      pointPosition--;
    }

    result.append(&buf[index + 1]);
  }

  if (value.isNegative)
    buf[index--] = '-';

  return &buf[index + 1];
}

struct Ieee754_64 {
  static constexpr uint8_t hiddenBitPos = 11;
  static constexpr uint8_t mantissaSize = 52;
  static constexpr int16_t exponentBias = 1023;
  static constexpr int16_t minExponent = -1022;
  static constexpr int16_t maxExponent = 1023;
  static constexpr uint16_t nanExponent = 0x7FF;
  static constexpr int16_t maxDecimalExponent = 308;
  static constexpr int16_t minDecimalExponent = -324;
  static constexpr uint64_t signMask = 0x8000000000000000ULL;
  static constexpr uint64_t mantissaMask = 0x000FFFFFFFFFFFFFULL;
  static constexpr uint64_t exponentMask = 0x7FF0000000000000ULL;
  static constexpr uint64_t hiddenBit = 0x0010000000000000ULL;
};

struct Ieee754_32 {
  static constexpr uint8_t hiddenBitPos = 8;
  static constexpr uint8_t mantissaSize = 23;
  static constexpr int8_t exponentBias = 127;
  static constexpr int8_t minExponent = -126;
  static constexpr int8_t maxExponent = 127;
  static constexpr uint8_t nanExponent = 0xFF;
  static constexpr int8_t maxDecimalExponent = 38;
  static constexpr int8_t minDecimalExponent = -45;
  static constexpr uint32_t signMask = 0x80000000U;
  static constexpr uint32_t mantissaMask = 0x007FFFFFU;
  static constexpr uint32_t exponentMask = 0x7F800000U;
  static constexpr uint32_t hiddenBit = 0x00800000U;
};

template <typename T,
          typename =
              typename std::enable_if<std::is_floating_point<T>::value>::type>
using Ieee754 =
    typename std::conditional<sizeof(T) == 8, Ieee754_64, Ieee754_32>::type;

template <typename T, typename TSignificant, typename TExponent>
T packBinaryFloat(FloatBase<TSignificant, TExponent> binaryFloat) {
  using ieee754 = Ieee754<T>;
  using significand_t = TSignificant;
  using exponent_t = TExponent;
  using bits_t =
      typename std::conditional<sizeof(T) == 8, uint64_t, uint32_t>::type;

  ARDUINOJSON_ASSERT(binaryFloat.normalized());
  ARDUINOJSON_ASSERT(!binaryFloat.isNaN);
  ARDUINOJSON_ASSERT(!binaryFloat.isInfinity);

  const significand_t signBit = binaryFloat.isNegative ? ieee754::signMask : 0;
  significand_t mantissa = binaryFloat.significand;
  // Float stores an integer significand (decimal point to the right),
  // whereas IEEE-754 stores a fractional significand (1.xxxxx). Converting
  // fractional to integer multiplies significand by 2^mantissaSize, so the
  // exponent must decrease by mantissaSize to preserve the value.
  exponent_t exponent =
      exponent_t(binaryFloat.exponent + ieee754::mantissaSize);

  if (mantissa == 0)
    return bit_cast<T>(signBit);

  // Move MSB to hidden bit location
  // Normalize and adjust exponent.
  mantissa >>= ieee754::hiddenBitPos;
  exponent = exponent_t(exponent + ieee754::hiddenBitPos);

  // Handle subnormal numbers
  if (exponent < ieee754::minExponent) {
    // Shift the mantissa to the right to make it subnormal
    int shift = ieee754::minExponent - exponent;
    // Ensure we don't shift more than the bit-width, as it's a UB
    assert(shift < static_cast<int>(sizeof(mantissa) * 8));
    mantissa >>= shift;
    return bit_cast<T>(signBit | mantissa);
  }

  // Remove hidden bit
  mantissa &= ~ieee754::hiddenBit;

  // Bias the exponent
  significand_t biasedExponent =
      static_cast<significand_t>(exponent + ieee754::exponentBias);

  // Infinity and NaN should have been handled above, so we should never hit the
  // max biased exponent here.
  assert(biasedExponent < ieee754::nanExponent);

  bits_t bits = signBit | (biasedExponent << ieee754::mantissaSize) | mantissa;
  return bit_cast<T>(bits);
}

template <typename T>
Float<T> unpackBinaryFloat(T value) {
  using binary_t = Float<T>;
  using ieee754 = Ieee754<T>;
  using significand_t = typename binary_t::significand_type;
  using exponent_t = typename binary_t::exponent_type;

  auto bits = bit_cast<significand_t>(value);

  exponent_t exponent = static_cast<exponent_t>(
      (bits & ieee754::exponentMask) >> ieee754::mantissaSize);
  significand_t mantissa = bits & ieee754::mantissaMask;

  binary_t binaryFloat;
  binaryFloat.isNegative = (bits & ieee754::signMask) != 0;
  if (exponent == ieee754::nanExponent) {
    binaryFloat.isNaN = mantissa != 0;
    binaryFloat.isInfinity = mantissa == 0;
    return binaryFloat;
  }

  binaryFloat.significand = mantissa;

  if (exponent != 0) {
    // Normalized number: add the hidden bit back and unbias the exponent
    binaryFloat.significand += ieee754::hiddenBit;
    exponent = exponent_t(exponent - ieee754::exponentBias);
  } else if (binaryFloat.significand) {
    // Subnormal number: the exponent is the minimum and there's no hidden bit
    exponent = ieee754::minExponent;

    // Shift  until it starts at the same position as a normalized number
    while ((binaryFloat.significand & ieee754::hiddenBit) == 0) {
      binaryFloat.significand <<= 1;
      exponent--;
    }
  } else {
    assert(exponent == 0);
    assert(binaryFloat.significand == 0);
  }

  // Normalize: move the most significant bit to the leftmost position.
  // This significantly improves the accuracy of future computations.
  constexpr int shift = sizeof(significand_t) * 8 - ieee754::mantissaSize - 1;
  binaryFloat.significand <<= shift;

  // Float stores an integer significand (decimal point to the right),
  // whereas IEEE-754 stores a fractional significand (1.xxxxx). Converting
  // fractional to integer multiplies significand by 2^mantissaSize, so the
  // exponent must decrease by mantissaSize to preserve the value.
  binaryFloat.exponent = exponent_t(exponent - shift - ieee754::mantissaSize);

  return binaryFloat;
}

template <typename T>
constexpr T constpow10(int k) {
  return k == 0 ? 1 : 10 * constpow10<T>(k - 1);
}

template <typename T>
Float<T> binaryToDecimalFloat(Float<T> binaryFloat) {
  Float<T> decimalFloat;
  using significand_t = typename Float<T>::significand_type;
  using exponent_t = typename Float<T>::exponent_type;

  decimalFloat.isNegative = binaryFloat.isNegative;
  decimalFloat.isNaN = binaryFloat.isNaN;
  decimalFloat.isInfinity = binaryFloat.isInfinity;

  if (decimalFloat.isNaN || decimalFloat.isInfinity ||
      binaryFloat.significand == 0)
    return decimalFloat;

  auto& cache = getCache(T());

  int16_t decimalExponent = 0;

  // Scale the floating point value so that the exponents lands in [0, 3]
  // Returns the scaling factor's power of 10
  //
  // or said differently:
  //
  // Finds E, so that X/10^E is in [0, 10] (or close enough)
  // Applies the multiplication and returns E.
  //
  // This is the core of the Grisu algorithm, but this version was modified to
  // reduce the memory footprint.

  // The exponent is too high, scale it down
  for (uint8_t i = 0; i < cache.size && binaryFloat.exponent > 3; i++) {
    // Exponent after multiplying by 10^-(2^N)
    auto newExponent =
        exponent_t(binaryFloat.exponent + cache.negativeExponents[i]);

    // Skip this power of 10 if it leads to a negative exponent
    // Allow some slack because the is too much distance between the cached
    // negative powers of ten
    if (newExponent < -3)
      continue;

    // We know we'll still be in the range, so let's apply the multiplication
    binaryFloat.exponent = newExponent;
    binaryFloat.significand =
        multiplyHigh(binaryFloat.significand, cache.negativeSignificands[i]);

    // Increase the power of 10 by 2^N
    const exponent_t exponentOffset = exponent_t(1 << (cache.size - 1 - i));
    decimalExponent = exponent_t(decimalExponent + exponentOffset);
  }

  // The exponent is too low, scale it up
  for (uint8_t i = 0; i < cache.size && binaryFloat.exponent < 0; i++) {
    // Exponent after multiplying by 10^(2^N)
    auto newExponent =
        exponent_t(binaryFloat.exponent + cache.positiveExponents[i]);

    // Skip this power of 10 if it leads to a too high exponent
    if (newExponent > 3)
      continue;

    // We know we'll still be in the range, so let's apply the multiplication
    binaryFloat.exponent = newExponent;
    binaryFloat.significand =
        multiplyHigh(binaryFloat.significand, cache.positiveSignificands[i]);

    // Increase the power of 10 by 2^N
    const exponent_t exponentOffset = exponent_t(1 << (cache.size - 1 - i));
    decimalExponent = exponent_t(decimalExponent - exponentOffset);
  }

  // binaryFloat should now be in the range [1, 10), or close enough
  assert(binaryFloat.exponent >= 0);
  assert(binaryFloat.exponent <= 3);

  // To reduce the number of digits, we divide by a large power ten (10^8)
  // We take this opportunity to cancel the remaining binary exponent
  const uint8_t divisorDigits = sizeof(T) == 8 ? 8 : 3;
  const significand_t divisor =
      constpow10<significand_t>(divisorDigits) >> binaryFloat.exponent;

  // Reduce number of digits and cancel the remaining binary exponent
  decimalFloat.significand = binaryFloat.significand / divisor;

  // Adjust for the division by 10^N above
  decimalFloat.exponent = exponent_t(decimalExponent + divisorDigits);

  // Round up if needed
  uint64_t rem = binaryFloat.significand % divisor;
  if (rem >= divisor / 2)
    decimalFloat.significand++;

  return decimalFloat;
}

template <typename T>
std::string floatingPointToString(const T& value) {
  auto binaryFloat = unpackBinaryFloat(value);
  if (std::isnan(value))
    assert(std::isnan(packBinaryFloat<T>(binaryFloat)));
  else
    assert(packBinaryFloat<T>(binaryFloat) == value);
  auto decimalFloat = binaryToDecimalFloat<T>(binaryFloat);
  return decimalToString(decimalFloat, abs(value) >= 1e7 || abs(value) <= 1e-5);
}

template <typename T>
inline Float<T> stringToDecimal(const char* s) {
  Float<T> result;

  switch (*s) {
    case '-':
      result.isNegative = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }

#if ARDUINOJSON_ENABLE_NAN
  if (*s == 'n' || *s == 'N') {
    result.isNaN = true;
    return result;
  }
#endif

#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    result.isInfinity = true;
    return result;
  }
#endif

  if (!isdigit(*s) && *s != '.') {
    result.isError = true;
    return result;
  }

  using significand_t = typename Float<T>::significand_type;
  using exponent_t = typename Float<T>::exponent_type;

  const significand_t maxSignificand = significand_t(-1);

  while (isdigit(*s)) {
    uint8_t digit = uint8_t(*s - '0');
    if (result.significand > maxSignificand / 10)
      break;
    result.significand *= 10;
    if (result.significand > maxSignificand - digit)
      break;
    result.significand += digit;
    s++;
  }

  if (*s == '\0')
    return result;

  // skip the remaining digits as they cannot fit in the significand
  exponent_t exponentOffset = 0;
  while (isdigit(*s)) {
    exponentOffset++;
    if (exponentOffset > 100) {
      // That's way to many digits!
      result.isError = true;
      return result;
    }
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (result.significand < maxSignificand / 10) {
        result.significand = result.significand * 10 + uint8_t(*s - '0');
        exponentOffset--;
        if (exponentOffset < -100) {
          // That's way to many digits!
          result.isError = true;
          return result;
        }
      }
      s++;
    }
  }

  if (*s == 'e' || *s == 'E') {
    s++;
    auto maxExponent = Ieee754<T>::maxDecimalExponent;
    bool exponentIsNegative = false;
    if (*s == '-') {
      exponentIsNegative = true;
      maxExponent = -Ieee754<T>::minDecimalExponent;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while (isdigit(*s)) {
      const char digit = char(*s - '0');
      result.exponent = exponent_t(result.exponent * 10 + digit);
      // Exponent overflow?
      if (result.exponent + exponentOffset > maxExponent) {
        // Return 0 or inf, depending of the sign
        if (exponentIsNegative)
          result.significand = 0;
        else
          result.isInfinity = true;
        return result;
      }
      s++;
    }
    if (exponentIsNegative)
      result.exponent = exponent_t(-result.exponent);
  }
  result.exponent = exponent_t(result.exponent + exponentOffset);

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0')
    result.isError = true;

  return result;
}

template <typename T>
Float<T> decimalToBinaryFloat(Float<T> decimalFloat) {
  Float<T> result;
  result.significand = decimalFloat.significand;
  result.isNegative = decimalFloat.isNegative;
  result.isNaN = decimalFloat.isNaN;
  result.isInfinity = decimalFloat.isInfinity;

  if (result.isNaN || result.isInfinity || result.significand == 0)
    return result;

  using exponent_t = typename Float<T>::exponent_type;

  // Push the most significant bit to the leftmost position.
  // This significantly improves the accuracy of future computations.
  result.normalize();

  const auto& cache = getCache(T());
  const auto* cacheExponents = decimalFloat.exponent < 0
                                   ? cache.negativeExponents
                                   : cache.positiveExponents;
  const auto* cacheSignificands = decimalFloat.exponent < 0
                                      ? cache.negativeSignificands
                                      : cache.positiveSignificands;

  if (decimalFloat.exponent < 0)
    decimalFloat.exponent = exponent_t(-decimalFloat.exponent);

  // TODO: we probably don't need to check exponent on each iteration
  for (uint8_t i = 0; i < cache.size && decimalFloat.exponent; i++) {
    int k =
        1 << (cache.size - 1 - i);  // TODO: try shifting left on each iteration
    if (decimalFloat.exponent & k) {
      result.significand =
          multiplyHigh(result.significand, cacheSignificands[i]);
      result.exponent = exponent_t(result.exponent + cacheExponents[i]);
    }
  }

  return result;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
