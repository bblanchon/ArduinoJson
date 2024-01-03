// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class Print {
 public:
  virtual ~Print() {}

  virtual size_t write(uint8_t) = 0;
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;

  size_t write(const char *str) {
    if (!str)
      return 0;
    return write(reinterpret_cast<const uint8_t *>(str), strlen(str));
  }

  size_t write(const char *buffer, size_t size) {
    return write(reinterpret_cast<const uint8_t *>(buffer), size);
  }
};

class Printable {
 public:
  virtual ~Printable() {}
  virtual size_t printTo(Print &p) const = 0;
};
