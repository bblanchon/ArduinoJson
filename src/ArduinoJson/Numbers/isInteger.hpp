// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/ctype.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline bool isInteger(const char* s) {
  if (!s || !*s) return false;
  if (issign(*s)) s++;
  while (isdigit(*s)) s++;
  return *s == '\0';
}
}  // namespace ARDUINOJSON_NAMESPACE
