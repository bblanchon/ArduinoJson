// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // size_t
#include <string.h>  // strcmp

namespace ARDUINOJSON_NAMESPACE {

class ZeroTerminatedRamStringConst {
 public:
  ZeroTerminatedRamStringConst(const char* str = 0) : _str(str) {}

  bool equals(const char* expected) const {
    const char* actual = _str;
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  // template <typename TMemoryPool>
  // const char* save(TMemoryPool*) const {
  //   return _str;
  // }

  size_t size() const {
    return strlen(_str);
  }

  const char* c_str() const {
    return _str;
  }

 protected:
  const char* _str;
};

inline ZeroTerminatedRamStringConst makeString(const char* str) {
  return ZeroTerminatedRamStringConst(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
