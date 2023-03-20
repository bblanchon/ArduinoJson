// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>

struct FailingAllocator : ArduinoJson::Allocator {
  static FailingAllocator* instance() {
    static FailingAllocator allocator;
    return &allocator;
  }

 private:
  FailingAllocator() = default;
  ~FailingAllocator() = default;

  void* allocate(size_t) override {
    return nullptr;
  }

  void deallocate(void*) override {}

  void* reallocate(void*, size_t) override {
    return nullptr;
  }
};
