// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

namespace ArduinoJson {
namespace Polyfills {

inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}

inline bool issign(char c) {
  return '-' == c || c == '+';
}
}
}
