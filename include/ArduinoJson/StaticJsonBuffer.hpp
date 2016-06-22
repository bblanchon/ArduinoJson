// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonBuffer.hpp"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace ArduinoJson {

// Implements a JsonBuffer with fixed memory allocation.
// The template paramenter CAPACITY specifies the capacity of the buffer in
// bytes.
template <size_t CAPACITY>
class StaticJsonBuffer : public JsonBuffer {
 public:
  explicit StaticJsonBuffer() : _size(0) {}

  size_t capacity() const {
    return CAPACITY;
  }
  size_t size() const {
    return _size;
  }

  virtual void* alloc(size_t bytes) {
    if (_size + bytes > CAPACITY) return NULL;
    void* p = &_buffer[_size];
    _size += round_size_up(bytes);
    return p;
  }

 private:
  uint8_t _buffer[CAPACITY];
  size_t _size;
};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
