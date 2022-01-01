// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wconversion"
#elif defined(__GNUC__)
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#    pragma GCC diagnostic push
#  endif
#  pragma GCC diagnostic ignored "-Wconversion"
#endif

#include <ArduinoJson/Numbers/Float.hpp>
#include <ArduinoJson/Polyfills/limits.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

// uint32 -> int32
// uint64 -> int32
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_unsigned<TIn>::value &&
                       is_integral<TOut>::value && sizeof(TOut) <= sizeof(TIn),
                   bool>::type
canConvertNumber(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest());
}

// uint32 -> int64
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_unsigned<TIn>::value &&
                       is_integral<TOut>::value && sizeof(TIn) < sizeof(TOut),
                   bool>::type
canConvertNumber(TIn) {
  return true;
}

// uint32 -> float
// int32 -> float
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_floating_point<TOut>::value,
                   bool>::type
canConvertNumber(TIn) {
  return true;
}

// int64 -> int32
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_signed<TIn>::value &&
                       is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TOut) < sizeof(TIn),
                   bool>::type
canConvertNumber(TIn value) {
  return value >= TIn(numeric_limits<TOut>::lowest()) &&
         value <= TIn(numeric_limits<TOut>::highest());
}

// int32 -> int32
// int32 -> int64
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_signed<TIn>::value &&
                       is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TIn) <= sizeof(TOut),
                   bool>::type
canConvertNumber(TIn) {
  return true;
}

// int32 -> uint32
// int32 -> uint64
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_signed<TIn>::value &&
                       is_integral<TOut>::value && is_unsigned<TOut>::value &&
                       sizeof(TOut) >= sizeof(TIn),
                   bool>::type
canConvertNumber(TIn value) {
  if (value < 0)
    return false;
  return TOut(value) <= numeric_limits<TOut>::highest();
}

// int32 -> uint16
template <typename TOut, typename TIn>
typename enable_if<is_integral<TIn>::value && is_signed<TIn>::value &&
                       is_integral<TOut>::value && is_unsigned<TOut>::value &&
                       sizeof(TOut) < sizeof(TIn),
                   bool>::type
canConvertNumber(TIn value) {
  if (value < 0)
    return false;
  return value <= TIn(numeric_limits<TOut>::highest());
}

// float -> int32
// float -> int64
template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TIn>::value &&
                       !is_floating_point<TOut>::value,
                   bool>::type
canConvertNumber(TIn value) {
  return value >= numeric_limits<TOut>::lowest() &&
         value <= numeric_limits<TOut>::highest();
}

template <typename TOut, typename TIn>
TOut convertNumber(TIn value) {
  return canConvertNumber<TOut>(value) ? TOut(value) : 0;
}
}  // namespace ARDUINOJSON_NAMESPACE

#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#    pragma GCC diagnostic pop
#  endif
#endif
