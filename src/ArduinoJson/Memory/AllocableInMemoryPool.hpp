// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "MemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

class AllocableInMemoryPool {
 public:
  void *operator new(size_t n, MemoryPool *memoryPool) NOEXCEPT {
    return memoryPool->alloc(n);
  }

  void operator delete(void *, MemoryPool *)NOEXCEPT {}
};
}  // namespace ARDUINOJSON_NAMESPACE
