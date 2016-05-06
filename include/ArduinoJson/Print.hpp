// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifndef ARDUINO

#include <stddef.h>
#include <stdint.h>

namespace ArduinoJson {
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

  size_t println() { return write('\r') + write('\n'); }
};
}

#else

#include <Print.h>

#endif
