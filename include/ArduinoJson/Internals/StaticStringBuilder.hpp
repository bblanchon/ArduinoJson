// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a char[]
class StaticStringBuilder : public Print {
 public:
  StaticStringBuilder(char *buf, size_t size)
      : buffer(buf), capacity(size - 1), length(0) {
    buffer[0] = '\0';
  }

  virtual size_t write(uint8_t c);

 private:
  char *buffer;
  size_t capacity;
  size_t length;
};
}
}
