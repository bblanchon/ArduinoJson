// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

inline int8_t safe_strcmp(const char* a, const char* b) {
  if (a == b) return 0;
  if (!a) return -1;
  if (!b) return 1;
  return static_cast<int8_t>(strcmp(a, b));
}

inline int8_t safe_strncmp(const char* a, const char* b, size_t n) {
  if (a == b) return 0;
  if (!a) return -1;
  if (!b) return 1;
  return static_cast<int8_t>(strncmp(a, b, n));
}
}  // namespace ARDUINOJSON_NAMESPACE
