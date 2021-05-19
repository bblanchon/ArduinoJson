// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

namespace ARDUINOJSON_NAMESPACE {

// clang-format off
template <typename T>
struct is_integral : integral_constant<bool,
    is_same<typename remove_cv<T>::type, signed char>::value ||
    is_same<typename remove_cv<T>::type, unsigned char>::value ||
    is_same<typename remove_cv<T>::type, signed short>::value ||
    is_same<typename remove_cv<T>::type, unsigned short>::value ||
    is_same<typename remove_cv<T>::type, signed int>::value ||
    is_same<typename remove_cv<T>::type, unsigned int>::value ||
    is_same<typename remove_cv<T>::type, signed long>::value ||
    is_same<typename remove_cv<T>::type, unsigned long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
    is_same<typename remove_cv<T>::type, signed long long>::value ||
    is_same<typename remove_cv<T>::type, unsigned long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
    is_same<typename remove_cv<T>::type, signed __int64>::value ||
    is_same<typename remove_cv<T>::type, unsigned __int64>::value ||
#endif
    is_same<typename remove_cv<T>::type, char>::value ||
    is_same<typename remove_cv<T>::type, bool>::value> {};
// clang-format on
}  // namespace ARDUINOJSON_NAMESPACE
