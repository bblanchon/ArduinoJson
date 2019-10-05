// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

namespace Utf8 {
template <typename TStringBuilder>
inline void encodeCodepoint(uint16_t codepoint, TStringBuilder &str) {
  if (codepoint < 0x80) {
    str.append(char(codepoint));
    return;
  }

  if (codepoint >= 0x00000800) {
    str.append(char(0xe0 /*0b11100000*/ | (codepoint >> 12)));
    str.append(char(((codepoint >> 6) & 0x3f /*0b00111111*/) | 0x80));
  } else {
    str.append(char(0xc0 /*0b11000000*/ | (codepoint >> 6)));
  }
  str.append(char((codepoint & 0x3f /*0b00111111*/) | 0x80));
}
}  // namespace Utf8
}  // namespace ARDUINOJSON_NAMESPACE
