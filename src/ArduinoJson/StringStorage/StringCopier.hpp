// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringCopier {
 public:
  void startString(MemoryPool* pool) {
    _slot = pool->allocExpandableString();
    _size = 0;
  }

  void commit(MemoryPool* pool) {
    ARDUINOJSON_ASSERT(_slot.value);
    pool->freezeString(_slot, _size);
  }

  void append(const char* s) {
    while (*s) append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0) append(*s++);
  }

  void append(char c) {
    if (!_slot.value)
      return;

    if (_size >= _slot.size) {
      _slot.value = 0;
      return;
    }

    _slot.value[_size++] = c;
  }

  bool isValid() {
    return _slot.value != 0;
  }

  const char* c_str() {
    return _slot.value;
  }

 private:
  size_t _size;
  StringSlot _slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
