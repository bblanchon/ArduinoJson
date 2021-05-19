// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Numbers/Integer.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

enum CompareResult {
  COMPARE_RESULT_DIFFER = 0,
  COMPARE_RESULT_EQUAL = 1,
  COMPARE_RESULT_GREATER = 2,
  COMPARE_RESULT_LESS = 4,

  COMPARE_RESULT_GREATER_OR_EQUAL = 3,
  COMPARE_RESULT_LESS_OR_EQUAL = 5
};

template <typename T>
CompareResult arithmeticCompare(const T &lhs, const T &rhs) {
  if (lhs < rhs)
    return COMPARE_RESULT_LESS;
  else if (lhs > rhs)
    return COMPARE_RESULT_GREATER;
  else
    return COMPARE_RESULT_EQUAL;
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                           sizeof(T1) < sizeof(T2),
                       int  // Using int instead of void to avoid C2572 on
                            // Visual Studio 2012, 2013, and 2015
                       >::type * = 0) {
  return arithmeticCompare<T2>(static_cast<T2>(lhs), rhs);
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       sizeof(T2) < sizeof(T1)>::type * = 0) {
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_signed<T1>::value == is_signed<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_unsigned<T1>::value && is_signed<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  if (rhs < 0)
    return COMPARE_RESULT_GREATER;
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_signed<T1>::value && is_unsigned<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  if (lhs < 0)
    return COMPARE_RESULT_LESS;
  return arithmeticCompare<T2>(static_cast<T2>(lhs), rhs);
}

template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_floating_point<T1>::value ||
                       is_floating_point<T2>::value>::type * = 0) {
  return arithmeticCompare<double>(static_cast<double>(lhs),
                                   static_cast<double>(rhs));
}

template <typename T2>
CompareResult arithmeticCompareNegateLeft(
    UInt, const T2 &, typename enable_if<is_unsigned<T2>::value>::type * = 0) {
  return COMPARE_RESULT_LESS;
}

template <typename T2>
CompareResult arithmeticCompareNegateLeft(
    UInt lhs, const T2 &rhs,
    typename enable_if<is_signed<T2>::value>::type * = 0) {
  if (rhs > 0)
    return COMPARE_RESULT_LESS;
  return arithmeticCompare(-rhs, static_cast<T2>(lhs));
}

template <typename T1>
CompareResult arithmeticCompareNegateRight(
    const T1 &, UInt, typename enable_if<is_unsigned<T1>::value>::type * = 0) {
  return COMPARE_RESULT_GREATER;
}

template <typename T1>
CompareResult arithmeticCompareNegateRight(
    const T1 &lhs, UInt rhs,
    typename enable_if<is_signed<T1>::value>::type * = 0) {
  if (lhs > 0)
    return COMPARE_RESULT_GREATER;
  return arithmeticCompare(static_cast<T1>(rhs), -lhs);
}

}  // namespace ARDUINOJSON_NAMESPACE
