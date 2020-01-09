// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/pgmspace.hpp>

namespace ARDUINOJSON_NAMESPACE {

class FlashStringAdapter {
 public:
  FlashStringAdapter(const __FlashStringHelper* str) : _str(str) {}

  int8_t compare(const char* other) const {
    if (!other && !_str) return 0;
    if (!_str) return -1;
    if (!other) return 1;
    return int8_t(-strcmp_P(other, reinterpret_cast<const char*>(_str)));
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    size_t n = size() + 1;  // copy the terminator
    char* dup = pool->allocFrozenString(n);
    if (dup) memcpy_P(dup, reinterpret_cast<const char*>(_str), n);
    return dup;
  }

  const char* data() const {
    return 0;
  }

  size_t size() const {
    if (!_str) return 0;
    return strlen_P(reinterpret_cast<const char*>(_str));
  }

  bool isStatic() const {
    return false;
  }

 private:
  const __FlashStringHelper* _str;
};

inline FlashStringAdapter adaptString(const __FlashStringHelper* str) {
  return FlashStringAdapter(str);
}

template <>
struct IsString<const __FlashStringHelper*> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
