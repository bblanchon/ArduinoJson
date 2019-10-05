// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "type_identity.hpp"
namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct make_unsigned;

template <>
struct make_unsigned<char> : type_identity<unsigned char> {};

template <>
struct make_unsigned<signed char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<unsigned char> : type_identity<unsigned char> {};

template <>
struct make_unsigned<signed short> : type_identity<unsigned short> {};
template <>
struct make_unsigned<unsigned short> : type_identity<unsigned short> {};

template <>
struct make_unsigned<signed int> : type_identity<unsigned int> {};
template <>
struct make_unsigned<unsigned int> : type_identity<unsigned int> {};

template <>
struct make_unsigned<signed long> : type_identity<unsigned long> {};
template <>
struct make_unsigned<unsigned long> : type_identity<unsigned long> {};

#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct make_unsigned<signed long long> : type_identity<unsigned long long> {};
template <>
struct make_unsigned<unsigned long long> : type_identity<unsigned long long> {};
#endif

#if ARDUINOJSON_HAS_INT64
template <>
struct make_unsigned<signed __int64> : type_identity<unsigned __int64> {};
template <>
struct make_unsigned<unsigned __int64> : type_identity<unsigned __int64> {};
#endif
}  // namespace ARDUINOJSON_NAMESPACE
