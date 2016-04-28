// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifndef ARDUINO

#include "../Internals/JsonFloat.hpp"
#include "../Internals/JsonInteger.hpp"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER <= 1800
// snprintf has been added in Visual Studio 2015
#define ARDUINOJSON_SNPRINTF _snprintf
#else
#define ARDUINOJSON_SNPRINTF snprintf
#endif

// This class reproduces Arduino's Print class
class Print {
 public:
  virtual ~Print() {}

  virtual size_t write(uint8_t) = 0;

  size_t print(const char* s) {
    size_t n = 0;
    while (*s) {
      n += write(*s++);
    }
    return n;
  }

  size_t print(ArduinoJson::Internals::JsonFloat value, int digits = 2) {
    char tmp[32];

    // https://github.com/arduino/Arduino/blob/db8cbf24c99dc930b9ccff1a43d018c81f178535/hardware/arduino/sam/cores/arduino/Print.cpp#L220
    bool isBigDouble = value > 4294967040.0 || value < -4294967040.0;

    if (isBigDouble) {
      // Arduino's implementation prints "ovf"
      // We prefer using the scientific notation, since we have sprintf
      ARDUINOJSON_SNPRINTF(tmp, sizeof(tmp), "%g", value);
    } else {
      // Here we have the exact same output as Arduino's implementation
      ARDUINOJSON_SNPRINTF(tmp, sizeof(tmp), "%.*f", digits, value);
    }

    return print(tmp);
  }

  size_t print(ArduinoJson::Internals::JsonUInt value) {
    char buffer[22];

    uint8_t i = 0;
    do {
      buffer[i++] = static_cast<char>(value % 10 + '0');
      value /= 10;
    } while (value);

    size_t n = 0;
    while (i > 0) {
      n += write(buffer[--i]);
    }

    return n;
  }

  size_t println() { return write('\r') + write('\n'); }
};

#else

#include <Print.h>

#endif
