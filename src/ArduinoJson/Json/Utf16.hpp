// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
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

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

namespace Utf16 {
inline bool isHighSurrogate(uint16_t codeunit) {
  return codeunit >= 0xD800 && codeunit < 0xDC00;
}

inline bool isLowSurrogate(uint16_t codeunit) {
  return codeunit >= 0xDC00 && codeunit < 0xE000;
}

class Codepoint {
 public:
  Codepoint() : highSurrogate_(0), codepoint_(0) {}

  bool append(uint16_t codeunit) {
    if (isHighSurrogate(codeunit)) {
      highSurrogate_ = codeunit & 0x3FF;
      return false;
    }

    if (isLowSurrogate(codeunit)) {
      codepoint_ =
          uint32_t(0x10000 + ((highSurrogate_ << 10) | (codeunit & 0x3FF)));
      return true;
    }

    codepoint_ = codeunit;
    return true;
  }

  uint32_t value() const {
    return codepoint_;
  }

 private:
  uint16_t highSurrogate_;
  uint32_t codepoint_;
};
}  // namespace Utf16
ARDUINOJSON_END_PRIVATE_NAMESPACE

#if defined(__GNUC__)
#  if __GNUC__ >= 8
#    pragma GCC diagnostic pop
#  endif
#endif
