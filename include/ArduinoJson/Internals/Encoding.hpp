// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Print.hpp"

namespace ArduinoJson {
namespace Internals {

class Encoding {
 public:
  // Optimized for code size on a 8-bit AVR
  static char escapeChar(char c) {
    const char *p = _escapeTable;
    while (p[0] && p[1] != c) {
      p += 2;
    }
    return p[0];
  }

  // Optimized for code size on a 8-bit AVR
  static char unescapeChar(char c) {
    const char *p = _escapeTable + 4;
    for (;;) {
      if (p[0] == '\0') return c;
      if (p[0] == c) return p[1];
      p += 2;
    }
  }

 private:
  static const char _escapeTable[];
};
}
}
