// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Some libraries #define isnan() and isinf() so we need to check before
// using this name

#ifndef isnan
template <typename T>
bool isnan(T x) {
  return x != x;
}
#endif

#ifndef isinf
template <typename T>
bool isinf(T x) {
  return x != 0.0 && x * 2 == x;
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
