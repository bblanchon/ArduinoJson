// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdint.h>  // uint16_t, uint32_t

namespace ARDUINOJSON_NAMESPACE {

namespace Utf16 {
inline bool isHighSurrogate(uint16_t codeunit) {
  return codeunit >= 0xD800 && codeunit < 0xDC00;
}

inline bool isLowSurrogate(uint16_t codeunit) {
  return codeunit >= 0xDC00 && codeunit < 0xE000;
}

class Codepoint {
 public:
  bool append(uint16_t codeunit) {
    if (isHighSurrogate(codeunit)) {
      _highSurrogate = codeunit & 0x3FF;
      return false;
    }

    if (isLowSurrogate(codeunit)) {
      _codepoint =
          uint32_t(0x10000 + ((_highSurrogate << 10) | (codeunit & 0x3FF)));
      return true;
    }

    _codepoint = codeunit;
    return true;
  }

  uint32_t value() const {
    return _codepoint;
  }

 private:
  uint16_t _highSurrogate;
  uint32_t _codepoint;
};
}  // namespace Utf16
}  // namespace ARDUINOJSON_NAMESPACE
