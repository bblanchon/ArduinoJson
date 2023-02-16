// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringCopier {
 public:
  StringCopier(MemoryPool* pool) : _pool(pool) {}

  void startString() {
    _pool->getFreeZone(&_ptr, &_capacity);
    _size = 0;
    if (_capacity == 0)
      _pool->markAsOverflowed();
  }

  JsonString save() {
    ARDUINOJSON_ASSERT(_ptr);
    ARDUINOJSON_ASSERT(_size < _capacity);  // needs room for the terminator
    return JsonString(_pool->saveStringFromFreeZone(_size), _size,
                      JsonString::Copied);
  }

  void append(const char* s) {
    while (*s)
      append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0)
      append(*s++);
  }

  void append(char c) {
    if (_size + 1 < _capacity)
      _ptr[_size++] = c;
    else
      _pool->markAsOverflowed();
  }

  bool isValid() const {
    return !_pool->overflowed();
  }

  size_t size() const {
    return _size;
  }

  JsonString str() const {
    ARDUINOJSON_ASSERT(_ptr);
    ARDUINOJSON_ASSERT(_size < _capacity);
    _ptr[_size] = 0;
    return JsonString(_ptr, _size, JsonString::Copied);
  }

 private:
  MemoryPool* _pool;

  // These fields aren't initialized by the constructor but startString()
  //
  // NOLINTNEXTLINE(clang-analyzer-optin.cplusplus.UninitializedObject)
  char* _ptr;
  // NOLINTNEXTLINE(clang-analyzer-optin.cplusplus.UninitializedObject)
  size_t _size, _capacity;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
