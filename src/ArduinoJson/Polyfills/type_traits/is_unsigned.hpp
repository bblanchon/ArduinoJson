// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// clang-format off
template <typename T>
struct is_unsigned : integral_constant<bool,
    is_same<remove_cv_t<T>, unsigned char>::value ||
    is_same<remove_cv_t<T>, unsigned short>::value ||
    is_same<remove_cv_t<T>, unsigned int>::value ||
    is_same<remove_cv_t<T>, unsigned long>::value ||
    is_same<remove_cv_t<T>, unsigned long long>::value ||
    is_same<remove_cv_t<T>, bool>::value> {};
// clang-format on

ARDUINOJSON_END_PRIVATE_NAMESPACE
