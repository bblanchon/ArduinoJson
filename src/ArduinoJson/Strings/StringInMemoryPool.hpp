// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string.h>
#include "../Memory/MemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringInMemoryPool {
 public:
  StringInMemoryPool(char* s = 0) : _value(s) {}

  bool equals(const char* expected) const {
    if (!_value) return expected == 0;
    const char* actual = _value;
    if (actual == expected) return true;
    return strcmp(actual, expected) == 0;
  }

  char* save(void*) {
    return _value;
  }

  bool isNull() const {
    return !_value;
  }

  const char* c_str() const {
    return _value;
  }

 protected:
  char* _value;
};

}  // namespace ARDUINOJSON_NAMESPACE
