// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

#ifndef isdigit
inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}
#endif

inline bool issign(char c) {
  return '-' == c || c == '+';
}

#ifndef isspace
inline bool isspace(int c) {
  return c == ' ' || ('\t' <= c && c <= '\r');
}
#endif

ARDUINOJSON_END_PRIVATE_NAMESPACE
