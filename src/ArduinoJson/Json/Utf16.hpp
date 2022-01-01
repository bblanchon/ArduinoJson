// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdint.h>  // uint16_t, uint32_t

// The high surrogate may be uninitialized if the pair is invalid,
// we choose to ignore the problem to reduce the size of the code
// Garbage in => Garbage out
#if defined(__GNUC__)
#  if __GNUC__ >= 7
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#  endif
#endif

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
  Codepoint() : _highSurrogate(0), _codepoint(0) {}

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

#if defined(__GNUC__)
#  if __GNUC__ >= 8
#    pragma GCC diagnostic pop
#  endif
#endif
