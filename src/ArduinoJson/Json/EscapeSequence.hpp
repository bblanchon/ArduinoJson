// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class EscapeSequence {
 public:
  // Optimized for code size on a 8-bit AVR
  // Assumes the table is sorted in descending order
  static char escapeChar(char c) {
    const char* p = escapeTable(false) - 2;
    do {
      p += 2;
    } while (uint8_t(c) < uint8_t(p[0]));
    return c == p[0] ? p[1] : 0;
  }

  // Optimized for code size on a 8-bit AVR
  static char unescapeChar(char c) {
    const char* p = escapeTable(true);
    for (;;) {
      if (p[0] == '\0')
        return 0;
      if (p[1] == c)
        return p[0];
      p += 2;
    }
  }

 private:
  static const char* escapeTable(bool includeOptional) {
    static const char charMap[] = {
        // Optional chars: only used for deserialization
        47, '/',   // solidus
        39, '\'',  // single quote
        // Mandatory chars: used for deserialization and serialization
        // Sorted in descending order to allow early exit
        92, '\\',  // reverse solidus
        34, '"',   // double quote
        13, 'r',   // carriage return
        12, 'f',   // formfeed
        10, 'n',   // linefeed
        9, 't',    // horizontal tab
        8, 'b',    // backspace
        0, 0,      //
    };
    return &charMap[includeOptional ? 0 : 4];
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
