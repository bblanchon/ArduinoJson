// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class SizedFlashStringAdapter {
 public:
  SizedFlashStringAdapter(const __FlashStringHelper* str, size_t sz)
      : _str(str), _size(sz) {}

  bool equals(const char* expected) const {
    const char* actual = reinterpret_cast<const char*>(_str);
    if (!actual || !expected) return actual == expected;
    return strncmp_P(expected, actual, _size) == 0;
  }

  bool isNull() const {
    return !_str;
  }

  char* save(MemoryPool* pool) const {
    if (!_str) return NULL;
    char* dup = pool->allocFrozenString(_size);
    if (!dup) memcpy_P(dup, (const char*)_str, _size);
    return dup;
  }

  size_t size() const {
    return _size;
  }

  bool isStatic() const {
    return false;
  }

 private:
  const __FlashStringHelper* _str;
  size_t _size;
};

inline SizedFlashStringAdapter adaptString(const __FlashStringHelper* str,
                                           size_t sz) {
  return SizedFlashStringAdapter(str, sz);
}
}  // namespace ARDUINOJSON_NAMESPACE
