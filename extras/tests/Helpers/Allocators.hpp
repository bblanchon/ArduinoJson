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

class AllocatorLog {
 public:
  struct Allocate {
    Allocate(size_t s) : size(s) {}
    size_t size;
  };

  struct Reallocate {
    Reallocate(size_t s) : size(s) {}
    size_t size;
  };

  struct Deallocate {};

  AllocatorLog& operator<<(const Allocate& a) {
    _log << "allocate(" << a.size << ")\n";
    return *this;
  }

  AllocatorLog& operator<<(const Deallocate&) {
    _log << "deallocate()\n";
    return *this;
  }

  AllocatorLog& operator<<(const Reallocate& a) {
    _log << "reallocate(" << a.size << ")\n";
    return *this;
  }

  std::string str() const {
    auto s = _log.str();
    if (s.empty())
      return "(empty)";
    s.pop_back();  // remove the trailing '\n'
    return s;
  }

  bool operator==(const AllocatorLog& other) const {
    return str() == other.str();
  }

  friend std::ostream& operator<<(std::ostream& os, const AllocatorLog& log) {
    os << log.str();
    return os;
  }

 private:
  std::ostringstream _log;
};

class SpyingAllocator : public ArduinoJson::Allocator {
 public:
  virtual ~SpyingAllocator() {}

  void* allocate(size_t n) override {
    _log << AllocatorLog::Allocate(n);
    return malloc(n);
  }

  void deallocate(void* p) override {
    _log << AllocatorLog::Deallocate();
    free(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    _log << AllocatorLog::Reallocate(n);
    return realloc(ptr, n);
  }

  const AllocatorLog& log() const {
    return _log;
  }

 private:
  AllocatorLog _log;
};
