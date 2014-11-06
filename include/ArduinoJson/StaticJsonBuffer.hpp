// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonBuffer.hpp"

namespace ArduinoJson {

// Implements a JsonBuffer with fixed memory allocation.
// The template paramenter CAPACITY specifies the capacity of the buffer in
// bytes.
template <size_t CAPACITY>
class StaticJsonBuffer : public JsonBuffer {
 public:
  explicit StaticJsonBuffer() : _size(0) {}
  virtual ~StaticJsonBuffer() {}

  size_t capacity() const { return CAPACITY; }
  size_t size() const { return _size; }
  // void clear() { _size = 0; }

 protected:
  virtual void* alloc(size_t size) {
    if (_size + size > CAPACITY) return NULL;
    void* p = &_buffer[_size];
    _size += size;
    return p;
  }

 private:
  uint8_t _buffer[CAPACITY];
  size_t _size;
};
}
