// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class EscapeSequence {
 public:
  // Optimized for code size on a 8-bit AVR
  static char escapeChar(char c) {
    const char* p = escapeTable(true);
    while (p[0] && p[1] != c) {
      p += 2;
    }
    return p[0];
  }

  // Optimized for code size on a 8-bit AVR
  static char unescapeChar(char c) {
    const char* p = escapeTable(false);
    for (;;) {
      if (p[0] == '\0')
        return 0;
      if (p[0] == c)
        return p[1];
      p += 2;
    }
  }

 private:
  static const char* escapeTable(bool isSerializing) {
    return &"//''\"\"\\\\b\bf\fn\nr\rt\t"[isSerializing ? 4 : 0];
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
