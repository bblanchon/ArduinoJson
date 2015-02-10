// Copyright Benoit Blanchon 2014-2015
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
  DynamicJsonBuffer() : _next(NULL), _size(0) {}

  ~DynamicJsonBuffer() { delete _next; }

  size_t size() const { return _size + (_next ? _next->size() : 0); }

  size_t blockCount() const { return 1 + (_next ? _next->blockCount() : 0); }

  static const size_t BLOCK_CAPACITY = 32;

 protected:
  virtual void* alloc(size_t bytes) {
    if (canAllocInThisBlock(bytes))
      return allocInThisBlock(bytes);
    else if (canAllocInOtherBlocks(bytes))
      return allocInOtherBlocks(bytes);
    else
      return NULL;
  }

 private:
  bool canAllocInThisBlock(size_t bytes) const {
    return _size + bytes <= BLOCK_CAPACITY;
  }

  void* allocInThisBlock(size_t bytes) {
    void* p = _buffer + _size;
    _size += bytes;
    return p;
  }

  bool canAllocInOtherBlocks(size_t bytes) const {
    // by design a DynamicJsonBuffer can't alloc a block bigger than
    // BLOCK_CAPACITY
    return bytes <= BLOCK_CAPACITY;
  }

  void* allocInOtherBlocks(size_t bytes) {
    if (!_next) {
      _next = new DynamicJsonBuffer();
      if (!_next) return NULL;
    }
    return _next->alloc(bytes);
  }

  DynamicJsonBuffer* _next;
  size_t _size;
  uint8_t _buffer[BLOCK_CAPACITY];
};
}
