// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StaticStringWriter {
 public:
  StaticStringWriter(char* buf, size_t size) : end(buf + size), p(buf) {}

  size_t write(uint8_t c) {
    if (p >= end)
      return 0;
    *p++ = static_cast<char>(c);
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    char* begin = p;
    while (p < end && n > 0) {
      *p++ = static_cast<char>(*s++);
      n--;
    }
    return size_t(p - begin);
  }

 private:
  char* end;
  char* p;
};
}  // namespace ARDUINOJSON_NAMESPACE
