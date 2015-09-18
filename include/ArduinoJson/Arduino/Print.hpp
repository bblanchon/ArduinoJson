// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifndef ARDUINO

#include <stddef.h>
#include <stdint.h>

// This class reproduces Arduino's Print class
class Print {
 public:
  virtual ~Print() {}

  virtual size_t write(uint8_t) = 0;

  size_t print(const char[]);
  size_t print(double, int = 2);
  size_t print(long);
  size_t println();
};

#else

#include <Print.h>

#endif
