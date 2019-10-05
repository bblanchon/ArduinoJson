// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "integral_constant.hpp"

namespace ARDUINOJSON_NAMESPACE {

// A meta-function that return the type T without the const modifier
template <typename T>
struct is_const : false_type {};

template <typename T>
struct is_const<const T> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
