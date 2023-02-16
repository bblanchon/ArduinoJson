// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// clang-format off
template <typename T>
struct is_signed : integral_constant<bool,
    is_same<typename remove_cv<T>::type, char>::value ||
    is_same<typename remove_cv<T>::type, signed char>::value ||
    is_same<typename remove_cv<T>::type, signed short>::value ||
    is_same<typename remove_cv<T>::type, signed int>::value ||
    is_same<typename remove_cv<T>::type, signed long>::value ||
    is_same<typename remove_cv<T>::type, signed long long>::value ||
    is_same<typename remove_cv<T>::type, float>::value ||
    is_same<typename remove_cv<T>::type, double>::value> {};
// clang-format on

ARDUINOJSON_END_PRIVATE_NAMESPACE
