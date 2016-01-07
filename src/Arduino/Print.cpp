// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#ifndef ARDUINO

#include "../../include/ArduinoJson/Arduino/Print.hpp"

#include <math.h>   // for isnan() and isinf()
#include <stdio.h>  // for sprintf()

// only for GCC 4.9+
#if defined(__GNUC__) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

// Visual Studo 2012 didn't have isnan, nor isinf
#if defined(_MSC_VER) && _MSC_VER <= 1700
#include <float.h>
#define isnan(x) _isnan(x)
#define isinf(x) (!_finite(x))
#endif

size_t Print::print(const char s[]) {
  size_t n = 0;
  while (*s) {
    n += write(*s++);
  }
  return n;
}

size_t Print::print(double value, int digits) {
  // https://github.com/arduino/Arduino/blob/db8cbf24c99dc930b9ccff1a43d018c81f178535/hardware/arduino/sam/cores/arduino/Print.cpp#L218
  if (isnan(value)) return print("nan");
  if (isinf(value)) return print("inf");

  char tmp[32];

  // https://github.com/arduino/Arduino/blob/db8cbf24c99dc930b9ccff1a43d018c81f178535/hardware/arduino/sam/cores/arduino/Print.cpp#L220
  bool isBigDouble = value > 4294967040.0 || value < -4294967040.0;

  if (isBigDouble) {
    // Arduino's implementation prints "ovf"
    // We prefer trying to use scientific notation, since we have sprintf
    sprintf(tmp, "%g", value);
  } else {
    // Here we have the exact same output as Arduino's implementation
    sprintf(tmp, "%.*f", digits, value);
  }

  return print(tmp);
}

size_t Print::print(long value) {
  char tmp[32];
  sprintf(tmp, "%ld", value);
  return print(tmp);
}

size_t Print::print(int value) {
  char tmp[32];
  sprintf(tmp, "%d", value);
  return print(tmp);
}

size_t Print::println() { return write('\r') + write('\n'); }

#endif
