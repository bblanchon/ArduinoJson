// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonBuffer.hpp"

namespace ArduinoJson {

// Implements a JsonBuffer with dynamic memory allocation.
// You are strongly encouraged to consider using StaticJsonBuffer which is much
// more suitable for embedded systems.
class DynamicJsonBuffer : public JsonBuffer {
 public:
  explicit DynamicJsonBuffer() : _size(0) {}

  size_t size() const { return _size; }

  static const size_t BLOCK_CAPACITY = 32;

 protected:
  virtual void* alloc(size_t bytes) {
    if (bytes > BLOCK_CAPACITY) return NULL;
    void* p = _buffer + _size;
    _size += bytes;
    return p;
  }

  bool canStore(size_t bytes) {
    // by design a DynamicJsonBuffer can't alloc a block bigger than
    // BLOCK_CAPACITY
    return bytes < BLOCK_CAPACITY;
  }

  size_t _size;
  uint8_t _buffer[BLOCK_CAPACITY];
};
}
