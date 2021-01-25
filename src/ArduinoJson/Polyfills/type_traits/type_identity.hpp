// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "integral_constant.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct type_identity {
  typedef T type;
};
}  // namespace ARDUINOJSON_NAMESPACE
