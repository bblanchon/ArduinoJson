// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include "is_class.hpp"
#include "is_convertible.hpp"
#include "is_floating_point.hpp"
#include "is_integral.hpp"
#include "is_same.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct is_enum {
  static const bool value = is_convertible<T, int>::value &&
                            !is_class<T>::value && !is_integral<T>::value &&
                            !is_floating_point<T>::value;
};

}  // namespace ARDUINOJSON_NAMESPACE
