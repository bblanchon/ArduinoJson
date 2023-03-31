// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>

#include <sstream>

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

class SpyingAllocator : public ArduinoJson::Allocator {
 public:
  virtual ~SpyingAllocator() {}

  void* allocate(size_t n) override {
    _log << "A" << n;
    return malloc(n);
  }

  void deallocate(void* p) override {
    _log << "F";
    free(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    _log << "R" << n;
    return realloc(ptr, n);
  }

  std::string log() const {
    return _log.str();
  }

 private:
  std::ostringstream _log;
};
