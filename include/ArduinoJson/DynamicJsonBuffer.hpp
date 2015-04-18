// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonBuffer.hpp"

#include <stdlib.h>

namespace ArduinoJson {

// Forward declaration
namespace Internals {
struct DynamicJsonBufferBlock;
}

// Implements a JsonBuffer with dynamic memory allocation.
// You are strongly encouraged to consider using StaticJsonBuffer which is much
// more suitable for embedded systems.
class DynamicJsonBuffer : public JsonBuffer {
 public:
  DynamicJsonBuffer();
  ~DynamicJsonBuffer();

  size_t size() const;

 protected:
  virtual void* alloc(size_t bytes);

 private:
  typedef Internals::DynamicJsonBufferBlock Block;

  static const size_t FIRST_BLOCK_CAPACITY = 32;

  static Block* createBlock(size_t capacity);

  inline bool canAllocInHead(size_t bytes) const;
  inline void* allocInHead(size_t bytes);
  inline void addNewBlock();

  Block* _head;
};
}
