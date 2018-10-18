// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Strings/StringInMemoryPool.hpp"
#include "MemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringBuilder {
 public:
  explicit StringBuilder(MemoryPool* parent)
      : _parent(parent), _start(0), _size(0) {
    _start = _parent->alloc(1);
  }

  void append(char c) {
    _start = _parent->realloc(_start, _size + 1, _size + 2);
    if (_start) _start[_size++] = c;
  }

  StringInMemoryPool complete() {
    if (_start) _start[_size] = 0;
    return _start;
  }

 private:
  MemoryPool* _parent;
  char* _start;
  size_t _size;
};

}  // namespace ARDUINOJSON_NAMESPACE
