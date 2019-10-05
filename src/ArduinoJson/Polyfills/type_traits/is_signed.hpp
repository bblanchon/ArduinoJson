// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "integral_constant.hpp"
namespace ARDUINOJSON_NAMESPACE {

template <typename>
struct is_signed : false_type {};

template <>
struct is_signed<char> : true_type {};

template <>
struct is_signed<signed char> : true_type {};

template <>
struct is_signed<signed short> : true_type {};

template <>
struct is_signed<signed int> : true_type {};

template <>
struct is_signed<signed long> : true_type {};

template <>
struct is_signed<float> : true_type {};

template <>
struct is_signed<double> : true_type {};

#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_signed<signed long long> : true_type {};
#endif

#if ARDUINOJSON_HAS_INT64
template <>
struct is_signed<signed __int64> : true_type {};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
