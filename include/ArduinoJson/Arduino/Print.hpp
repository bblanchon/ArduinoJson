// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifndef ARDUINO

#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsIntegral.hpp"

#include <stddef.h>
#include <stdint.h>

// This class reproduces Arduino's Print class
class Print {
 public:
  virtual ~Print() {}

  virtual size_t write(uint8_t) = 0;

  size_t print(const char[]);
  size_t print(double, int = 2);

  template <typename TIntegral>
  typename ArduinoJson::TypeTraits::EnableIf<
      ArduinoJson::TypeTraits::IsIntegral<TIntegral>::value, size_t>::type
  print(TIntegral value) {
    // see http://clc-wiki.net/wiki/K%26R2_solutions:Chapter_3:Exercise_4
    char buffer[22];

    size_t n = 0;
    if (value < 0) {
      value = -value;
      n += write('-');
    }
    uint8_t i = 0;
    do {
      TIntegral digit = value % 10;
      value /= 10;
      buffer[i++] = static_cast<char>(digit >= 0 ? '0' + digit : '0' - digit);
    } while (value);

    while (i > 0) {
      n += write(buffer[--i]);
    }

    return n;
  }

  size_t println();
};

#else

#include <Print.h>

#endif
