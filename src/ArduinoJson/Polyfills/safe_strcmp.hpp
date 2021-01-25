// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdint.h>  // int8_t

namespace ARDUINOJSON_NAMESPACE {

inline int safe_strcmp(const char* a, const char* b) {
  if (a == b)
    return 0;
  if (!a)
    return -1;
  if (!b)
    return 1;
  return strcmp(a, b);
}

inline int safe_strncmp(const char* a, const char* b, size_t n) {
  if (a == b)
    return 0;
  if (!a)
    return -1;
  if (!b)
    return 1;
  return strncmp(a, b, n);
}
}  // namespace ARDUINOJSON_NAMESPACE
