// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Namespace.hpp>

#include <string.h>  // strcmp, strncmp

// Issue #1198: strcmp() implementation that returns a value larger than 8-bit

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

int strcmp(const char* a, const char* b) {
  int result = ::strcmp(a, b);
  if (result > 0)
    return 2147483647;
  if (result < 0)
    return -214748364;
  return 0;
}

int strncmp(const char* a, const char* b, size_t n) {
  int result = ::strncmp(a, b, n);
  if (result > 0)
    return 2147483647;
  if (result < 0)
    return -214748364;
  return 0;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
