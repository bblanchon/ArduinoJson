// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdlib.h>  // malloc, free

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class Allocator {
 public:
  virtual void* allocate(size_t size) = 0;
  virtual void deallocate(void* ptr) = 0;
  virtual void* reallocate(void* ptr, size_t new_size) = 0;

 protected:
  ~Allocator() = default;
};

namespace detail {
class DefaultAllocator : public Allocator {
 public:
  void* allocate(size_t size) override {
    return malloc(size);
  }

  void deallocate(void* ptr) override {
    free(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) override {
    return realloc(ptr, new_size);
  }

  static Allocator* instance() {
    static DefaultAllocator allocator;
    return &allocator;
  }

 private:
  DefaultAllocator() = default;
  ~DefaultAllocator() = default;
};
}  // namespace detail

ARDUINOJSON_END_PUBLIC_NAMESPACE
