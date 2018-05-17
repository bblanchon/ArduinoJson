// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "integral_constant.hpp"

namespace ArduinoJson {
namespace Internals {

// A meta-function that return the type T without the const modifier
template <typename T>
struct is_const : false_type {};

template <typename T>
struct is_const<const T> : true_type {};
}  // namespace Internals
}  // namespace ArduinoJson
