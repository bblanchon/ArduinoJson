// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

namespace ARDUINOJSON_NAMESPACE {

// clang-format off
template <typename T>
struct is_signed : integral_constant<bool, 
    is_same<typename remove_cv<T>::type, char>::value ||
    is_same<typename remove_cv<T>::type, signed char>::value ||
    is_same<typename remove_cv<T>::type, signed short>::value ||
    is_same<typename remove_cv<T>::type, signed int>::value ||
    is_same<typename remove_cv<T>::type, signed long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
    is_same<typename remove_cv<T>::type, signed long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
    is_same<typename remove_cv<T>::type, signed __int64>::value ||
#endif
    is_same<typename remove_cv<T>::type, float>::value ||
    is_same<typename remove_cv<T>::type, double>::value> {};
// clang-format on
}  // namespace ARDUINOJSON_NAMESPACE
