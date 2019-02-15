// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <string>

namespace ARDUINOJSON_NAMESPACE {

class StlStringAdapter {
 public:
  StlStringAdapter(const std::string& str) : _str(&str) {}

  char* save(MemoryPool* pool) const {
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy(dup, _str->c_str(), n);
    return dup;
  }

  bool isNull() const {
    return false;
  }

  bool equals(const char* expected) const {
    if (!expected) return false;
    return *_str == expected;
  }

  const char* data() const {
    return _str->data();
  }

  size_t size() const {
    return _str->size();
  }

  bool isStatic() const {
    return false;
  }

 private:
  const std::string* _str;
};

template <>
struct IsString<std::string> : true_type {};

inline StlStringAdapter adaptString(const std::string& str) {
  return StlStringAdapter(str);
}

}  // namespace ARDUINOJSON_NAMESPACE
