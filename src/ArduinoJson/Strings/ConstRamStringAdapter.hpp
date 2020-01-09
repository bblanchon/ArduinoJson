// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

#include <ArduinoJson/Polyfills/safe_strcmp.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ConstRamStringAdapter {
 public:
  ConstRamStringAdapter(const char* str = 0) : _str(str) {}

  int8_t compare(const char* other) const {
    return safe_strcmp(_str, other);
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  template <typename TMemoryPool>
  char* save(TMemoryPool*) const {
    return 0;
  }

  size_t size() const {
    if (!_str) return 0;
    return strlen(_str);
  }

  const char* data() const {
    return _str;
  }

  bool isStatic() const {
    return true;
  }

 protected:
  const char* _str;
};

inline ConstRamStringAdapter adaptString(const char* str) {
  return ConstRamStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
