// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#include "../Polyfills/limits.hpp"
#include "Float.hpp"
#include "FloatTraits.hpp"
#include "Integer.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && sizeof(TOut) <= sizeof(TIn),
                   bool>::type
canStorePositiveInteger(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest());
}

template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && sizeof(TIn) < sizeof(TOut),
                   bool>::type
canStorePositiveInteger(TIn) {
  return true;
}

template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, bool>::type
canStorePositiveInteger(TIn) {
  return true;
}

template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, bool>::type
canStoreNegativeInteger(TIn) {
  return true;
}

template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TOut) <= sizeof(TIn),
                   bool>::type
canStoreNegativeInteger(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest()) + 1;
}

template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TIn) < sizeof(TOut),
                   bool>::type
canStoreNegativeInteger(TIn) {
  return true;
}

template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_unsigned<TOut>::value,
                   bool>::type
canStoreNegativeInteger(TIn) {
  return false;
}

template <typename TOut, typename TIn>
TOut convertPositiveInteger(TIn value) {
  return canStorePositiveInteger<TOut>(value) ? TOut(value) : 0;
}

template <typename TOut, typename TIn>
TOut convertNegativeInteger(TIn value) {
  return canStoreNegativeInteger<TOut>(value) ? TOut(~value + 1) : 0;
}

template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, TOut>::type convertFloat(
    TIn value) {
  return TOut(value);
}

template <typename TOut, typename TIn>
typename enable_if<!is_floating_point<TOut>::value, TOut>::type convertFloat(
    TIn value) {
  return value >= numeric_limits<TOut>::lowest() &&
                 value <= numeric_limits<TOut>::highest()
             ? TOut(value)
             : 0;
}
}  // namespace ARDUINOJSON_NAMESPACE

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
