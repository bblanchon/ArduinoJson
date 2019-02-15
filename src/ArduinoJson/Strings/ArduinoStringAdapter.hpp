// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <WString.h>

namespace ARDUINOJSON_NAMESPACE {

class ArduinoStringAdapter {
 public:
  ArduinoStringAdapter(const ::String& str) : _str(&str) {}

  char* save(MemoryPool* pool) const {
    if (isNull()) return NULL;
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str->c_str(), n);
    return dup;
  }

  bool isNull() const {
    // Arduino's String::c_str() can return NULL
    return !_str->c_str();
  }

  bool equals(const char* expected) const {
    // Arduino's String::c_str() can return NULL
    const char* actual = _str->c_str();
    if (!actual || !expected) return actual == expected;
    return 0 == strcmp(actual, expected);
  }

  const char* data() const {
    return _str->c_str();
  }

  size_t size() const {
    return _str->length();
  }

  bool isStatic() const {
    return false;
  }

 private:
  const ::String* _str;
};

template <>
struct IsString< ::String> : true_type {};

template <>
struct IsString< ::StringSumHelper> : true_type {};

inline ArduinoStringAdapter adaptString(const ::String& str) {
  return ArduinoStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
