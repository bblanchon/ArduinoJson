// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "integral_constant.hpp"
#include "is_same.hpp"
#include "remove_cv.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <class T>
struct is_floating_point
    : integral_constant<
          bool,  //
          is_same<float, typename remove_cv<T>::type>::value ||
              is_same<double, typename remove_cv<T>::type>::value> {};

}  // namespace ARDUINOJSON_NAMESPACE
