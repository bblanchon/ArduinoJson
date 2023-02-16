// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <class T>
struct is_floating_point
    : integral_constant<
          bool,  //
          is_same<float, typename remove_cv<T>::type>::value ||
              is_same<double, typename remove_cv<T>::type>::value> {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
