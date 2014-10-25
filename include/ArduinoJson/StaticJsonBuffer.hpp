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

  int capacity() { return CAPACITY; }

  int size() { return _size; }

 protected:
  virtual void *allocateNode() {
    if (_size >= CAPACITY) return 0;

    return &_buffer[_size++];
  }

 private:
  Internals::JsonNode _buffer[CAPACITY];
  int _size;
};
}
