// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// clang-format off
template <typename T>
struct is_integral : integral_constant<bool,
    is_same<remove_cv_t<T>, signed char>::value ||
    is_same<remove_cv_t<T>, unsigned char>::value ||
    is_same<remove_cv_t<T>, signed short>::value ||
    is_same<remove_cv_t<T>, unsigned short>::value ||
    is_same<remove_cv_t<T>, signed int>::value ||
    is_same<remove_cv_t<T>, unsigned int>::value ||
    is_same<remove_cv_t<T>, signed long>::value ||
    is_same<remove_cv_t<T>, unsigned long>::value ||
    is_same<remove_cv_t<T>, signed long long>::value ||
    is_same<remove_cv_t<T>, unsigned long long>::value ||
    is_same<remove_cv_t<T>, char>::value ||
    is_same<remove_cv_t<T>, bool>::value> {};
// clang-format on

ARDUINOJSON_END_PRIVATE_NAMESPACE
