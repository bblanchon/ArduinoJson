// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <WString.h>

namespace ARDUINOJSON_NAMESPACE {

template <>
class Writer< ::String, void> {
 public:
  explicit Writer(::String &str) : _str(&str) {}

  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    // CAUTION: Arduino String doesn't have append()
    // and old version doesn't have size() either
    _str->reserve(_str->length() + n);
    while (n > 0) {
      _str->operator+=(static_cast<char>(*s++));
      n--;
    }
    return n;
  }

 private:
  ::String *_str;
};

}  // namespace ARDUINOJSON_NAMESPACE
