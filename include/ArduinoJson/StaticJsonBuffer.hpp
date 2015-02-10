// Copyright Benoit Blanchon 2014-2015
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

  size_t capacity() const { return CAPACITY; }
  size_t size() const { return _size; }

 protected:
  virtual void* alloc(size_t bytes) {
    if (_size + bytes > CAPACITY) return NULL;
    void* p = &_buffer[_size];
    _size += bytes;
    return p;
  }

 private:
  uint8_t _buffer[CAPACITY];
  size_t _size;
};
}
