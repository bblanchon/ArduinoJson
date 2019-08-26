// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringBuilder {
 public:
  explicit StringBuilder(MemoryPool* parent) : _parent(parent), _size(0) {
    _slot = _parent->allocExpandableString();
  }

  void append(const char* s) {
    while (*s) append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0) append(*s++);
  }

  void append(char c) {
    if (!_slot.value) return;

    if (_size >= _slot.size) {
      _slot.value = 0;
      return;
    }

    _slot.value[_size++] = c;
  }

  char* complete() {
    append('\0');
    if (_slot.value) {
      _parent->freezeString(_slot, _size);
    }
    return _slot.value;
  }

 private:
  MemoryPool* _parent;
  size_t _size;
  StringSlot _slot;
};

}  // namespace ARDUINOJSON_NAMESPACE
