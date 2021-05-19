// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringCopier {
 public:
  StringCopier(MemoryPool& pool) : _pool(&pool) {}

  void startString() {
    _pool->getFreeZone(&_ptr, &_capacity);
    _size = 0;
  }

  const char* save() {
    ARDUINOJSON_ASSERT(_ptr);
    return _pool->saveStringFromFreeZone(_size);
  }

  void append(const char* s) {
    while (*s) append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0) append(*s++);
  }

  void append(char c) {
    if (!_ptr)
      return;

    if (_size >= _capacity) {
      _ptr = 0;
      _pool->markAsOverflowed();
      return;
    }

    _ptr[_size++] = c;
  }

  bool isValid() {
    return _ptr != 0;
  }

  const char* c_str() {
    return _ptr;
  }

  typedef storage_policies::store_by_copy storage_policy;

 private:
  MemoryPool* _pool;
  char* _ptr;
  size_t _size;
  size_t _capacity;
};
}  // namespace ARDUINOJSON_NAMESPACE
