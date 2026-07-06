// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <string>

// the space before _s is required by GCC 4.8
inline std::string operator"" _s(const char* str, size_t len) {
  return std::string(str, len);
}
