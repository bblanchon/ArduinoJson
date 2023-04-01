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
    Reallocate(size_t s1, size_t s2) : oldSize(s1), newSize(s2) {}
    size_t oldSize, newSize;
  };

  struct Deallocate {
    Deallocate(size_t s) : size(s) {}
    size_t size;
  };

  AllocatorLog& operator<<(const Allocate& a) {
    _log << "allocate(" << a.size << ")\n";
    return *this;
  }

  AllocatorLog& operator<<(const Deallocate& d) {
    _log << "deallocate(" << d.size << ")\n";
    return *this;
  }

  AllocatorLog& operator<<(const Reallocate& a) {
    _log << "reallocate(" << a.oldSize << ", " << a.newSize << ")\n";
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
    auto block = reinterpret_cast<AllocatedBlock*>(
        malloc(sizeof(AllocatedBlock) + n - 1));
    block->size = n;
    return block->payload;
  }

  void deallocate(void* p) override {
    auto block = AllocatedBlock::fromPayload(p);
    _log << AllocatorLog::Deallocate(block->size);
    free(block);
  }

  void* reallocate(void* p, size_t n) override {
    auto block = AllocatedBlock::fromPayload(p);
    _log << AllocatorLog::Reallocate(block->size, n);
    block = reinterpret_cast<AllocatedBlock*>(
        realloc(block, sizeof(AllocatedBlock) + n - 1));
    block->size = n;
    return block->payload;
  }

  const AllocatorLog& log() const {
    return _log;
  }

 private:
  struct AllocatedBlock {
    size_t size;
    char payload[1];

    static AllocatedBlock* fromPayload(void* p) {
      return reinterpret_cast<AllocatedBlock*>(
          // Cast to void* to silence "cast increases required alignment of
          // target type [-Werror=cast-align]"
          reinterpret_cast<void*>(reinterpret_cast<char*>(p) -
                                  offsetof(AllocatedBlock, payload)));
    }
  };

  AllocatorLog _log;
};
