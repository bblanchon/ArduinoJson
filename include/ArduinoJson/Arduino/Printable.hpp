// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifndef ARDUINO

#include <stddef.h>

class Print;

class Printable {
 public:
  virtual size_t printTo(Print &p) const = 0;
};

#else

#include <Printable.h>

#endif
