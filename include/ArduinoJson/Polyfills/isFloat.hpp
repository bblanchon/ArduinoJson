// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "./ctype.hpp"

namespace ArduinoJson {
namespace Polyfills {

inline bool isFloat(const char* s) {
  if (!s) return false;

  if (!strcmp(s, "NaN")) return true;
  if (issign(*s)) s++;
  if (!strcmp(s, "Infinity")) return true;

  while (isdigit(*s)) s++;

  bool has_dot = *s == '.';
  if (has_dot) {
    s++;
    while (isdigit(*s)) s++;
  }

  bool has_exponent = *s == 'e' || *s == 'E';
  if (has_exponent) {
    s++;
    if (issign(*s)) s++;
    if (!isdigit(*s)) return false;
    while (isdigit(*s)) s++;
  }

  return (has_dot || has_exponent) && *s == '\0';
}
}
}
