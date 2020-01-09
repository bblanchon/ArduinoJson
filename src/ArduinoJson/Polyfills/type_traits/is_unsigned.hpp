// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "integral_constant.hpp"
namespace ARDUINOJSON_NAMESPACE {

template <typename>
struct is_unsigned : false_type {};

template <>
struct is_unsigned<bool> : true_type {};

template <>
struct is_unsigned<unsigned char> : true_type {};

template <>
struct is_unsigned<unsigned short> : true_type {};

template <>
struct is_unsigned<unsigned int> : true_type {};

template <>
struct is_unsigned<unsigned long> : true_type {};

#if ARDUINOJSON_HAS_INT64
template <>
struct is_unsigned<unsigned __int64> : true_type {};
#endif

#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_unsigned<unsigned long long> : true_type {};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
