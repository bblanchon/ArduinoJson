// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

#ifndef isdigit
inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}
#endif

inline bool issign(char c) {
  return '-' == c || c == '+';
}
}  // namespace ARDUINOJSON_NAMESPACE
