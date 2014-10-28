// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "JsonBuffer.hpp"

namespace ArduinoJson {
template <int CAPACITY>
class StaticJsonBuffer : public JsonBuffer {
 public:
  explicit StaticJsonBuffer() : _size(0) {}

  virtual ~StaticJsonBuffer() {}

  int capacity() const { return CAPACITY; }

  int size() const { return _size; }

  void clear() { _size = 0; }

 protected:
  virtual void* alloc(size_t size) {
    if (_size + size > CAPACITY) return NULL;
    void* p = &_buffer[_size];
    _size += size;
    return p;
  }

 private:
  char _buffer[CAPACITY];
  int _size;
};
}
