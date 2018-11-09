// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Strings/StringInMemoryPool.hpp"
#include "MemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringBuilder {
 public:
  typedef StringInMemoryPool StringType;

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
    if (!_slot) return;

    if (_size >= _slot->size) {
      _slot = _parent->expandString(_slot);
      if (!_slot) return;
    }

    _slot->value[_size++] = c;
  }

  StringType complete() {
    append('\0');
    if (_slot) {
      _parent->freezeString(_slot, _size);
    }
    return _slot;
  }

 private:
  MemoryPool* _parent;
  size_t _size;
  StringSlot* _slot;
};

}  // namespace ARDUINOJSON_NAMESPACE
