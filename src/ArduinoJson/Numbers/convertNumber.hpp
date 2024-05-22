// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wconversion"
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
#endif

#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// uint32 -> int32
// uint64 -> int32
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_unsigned<TIn>::value &&
                is_integral<TOut>::value && sizeof(TOut) <= sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest());
}

// uint32 -> int64
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_unsigned<TIn>::value &&
                is_integral<TOut>::value && sizeof(TIn) < sizeof(TOut),
            bool>
canConvertNumber(TIn) {
  return true;
}

// uint32 -> float
// int32 -> float
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_floating_point<TOut>::value, bool>
canConvertNumber(TIn) {
  return true;
}

// int64 -> int32
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_signed<TIn>::value &&
                is_integral<TOut>::value && is_signed<TOut>::value &&
                sizeof(TOut) < sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  return value >= TIn(numeric_limits<TOut>::lowest()) &&
         value <= TIn(numeric_limits<TOut>::highest());
}

// int32 -> int32
// int32 -> int64
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_signed<TIn>::value &&
                is_integral<TOut>::value && is_signed<TOut>::value &&
                sizeof(TIn) <= sizeof(TOut),
            bool>
canConvertNumber(TIn) {
  return true;
}

// int32 -> uint32
// int32 -> uint64
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_signed<TIn>::value &&
                is_integral<TOut>::value && is_unsigned<TOut>::value &&
                sizeof(TOut) >= sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  if (value < 0)
    return false;
  return TOut(value) <= numeric_limits<TOut>::highest();
}

// int32 -> uint16
template <typename TOut, typename TIn>
enable_if_t<is_integral<TIn>::value && is_signed<TIn>::value &&
                is_integral<TOut>::value && is_unsigned<TOut>::value &&
                sizeof(TOut) < sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  if (value < 0)
    return false;
  return value <= TIn(numeric_limits<TOut>::highest());
}

// float32 -> int16
// float64 -> int32
template <typename TOut, typename TIn>
enable_if_t<is_floating_point<TIn>::value && is_integral<TOut>::value &&
                sizeof(TOut) < sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  return value >= numeric_limits<TOut>::lowest() &&
         value <= numeric_limits<TOut>::highest();
}

// float32 -> int32
// float32 -> uint32
// float32 -> int64
// float32 -> uint64
// float64 -> int64
// float64 -> uint64
template <typename TOut, typename TIn>
enable_if_t<is_floating_point<TIn>::value && is_integral<TOut>::value &&
                sizeof(TOut) >= sizeof(TIn),
            bool>
canConvertNumber(TIn value) {
  // Avoid error "9.22337e+18 is outside the range of representable values of
  // type 'long'"
  return value >= numeric_limits<TOut>::lowest() &&
         value <= FloatTraits<TIn>::template highest_for<TOut>();
}

template <typename TOut, typename TIn>
TOut convertNumber(TIn value) {
  return canConvertNumber<TOut>(value) ? TOut(value) : 0;
}
ARDUINOJSON_END_PRIVATE_NAMESPACE

#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#endif
