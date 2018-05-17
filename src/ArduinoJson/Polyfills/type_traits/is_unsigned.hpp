// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "integral_constant.hpp"
namespace ArduinoJson {
namespace Internals {

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

#if ARDUINOJSON_USE_LONG_LONG
template <>
struct is_unsigned<unsigned long long> : true_type {};
#endif
#if ARDUINOJSON_USE_INT64
template <>
struct is_unsigned<unsigned __int64> : true_type {};
#endif
}  // namespace Internals
}  // namespace ArduinoJson
