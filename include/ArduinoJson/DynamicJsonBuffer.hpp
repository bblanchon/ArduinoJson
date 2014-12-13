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

 protected:
  virtual void* alloc(size_t bytes) {
    _size += bytes;
    return NULL;
  }

  size_t _size;
};
}
