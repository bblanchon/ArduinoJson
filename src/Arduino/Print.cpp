// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#ifndef ARDUINO

#include "ArduinoJson/Arduino/Print.hpp"
#include <cstdio>

size_t Print::print(const char s[]) {
  size_t n = 0;
  while (*s) {
    n += write(*s++);
  }
  return n;
}

size_t Print::print(double value, int digits) {
  char tmp[32];
  sprintf(tmp, "%.*g", digits + 1, value);
  return print(tmp);
}

size_t Print::print(long value) {
  char tmp[32];
  sprintf(tmp, "%ld", value);
  return print(tmp);
}

size_t Print::println() { return write('\r') + write('\n'); }

#endif