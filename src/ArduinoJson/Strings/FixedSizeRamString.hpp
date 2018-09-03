// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string.h>  // strcmp

namespace ArduinoJson {
namespace Internals {

class FixedSizeRamString {
 public:
  FixedSizeRamString(const char* str, size_t n) : _str(str), _size(n) {}

  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strcmp(actual, expected) == 0;
  }

  bool is_null() const {
    return !_str;
  }

  template <typename Buffer>
  const char* save(Buffer* memoryPool) const {
    if (!_str) return NULL;
    void* dup = memoryPool->alloc(_size);
    if (!dup) return NULL;
    memcpy(dup, _str, _size);
    return static_cast<const char*>(dup);
  }

  size_t size() const {
    return strlen(reinterpret_cast<const char*>(_str));
  }

 private:
  const char* _str;
  size_t _size;
};

template <typename TChar>
inline FixedSizeRamString makeString(const TChar* str, size_t size) {
  return FixedSizeRamString(reinterpret_cast<const char*>(str), size);
}

}  // namespace Internals
}  // namespace ArduinoJson
