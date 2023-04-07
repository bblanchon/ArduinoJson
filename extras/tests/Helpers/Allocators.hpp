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
  static std::string Allocate(size_t s) {
    char buffer[32];
    sprintf(buffer, "allocate(%zu)", s);
    return buffer;
  }

  static std::string AllocateFail(size_t s) {
    char buffer[32];
    sprintf(buffer, "allocate(%zu) -> nullptr", s);
    return buffer;
  }

  static std::string Reallocate(size_t s1, size_t s2) {
    char buffer[32];
    sprintf(buffer, "reallocate(%zu, %zu)", s1, s2);
    return buffer;
  };

  static std::string ReallocateFail(size_t s1, size_t s2) {
    char buffer[32];
    sprintf(buffer, "reallocate(%zu, %zu) -> nullptr", s1, s2);
    return buffer;
  };

  static std::string Deallocate(size_t s) {
    char buffer[32];
    sprintf(buffer, "deallocate(%zu)", s);
    return buffer;
  };

  AllocatorLog& operator<<(const std::string& s) {
    _log << s << "\n";
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
  SpyingAllocator(
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : _upstream(upstream) {}
  virtual ~SpyingAllocator() {}

  void* allocate(size_t n) override {
    auto block = reinterpret_cast<AllocatedBlock*>(
        _upstream->allocate(sizeof(AllocatedBlock) + n - 1));
    if (block) {
      _log << AllocatorLog::Allocate(n);
      block->size = n;
      return block->payload;
    } else {
      _log << AllocatorLog::AllocateFail(n);
      return nullptr;
    }
  }

  void deallocate(void* p) override {
    auto block = AllocatedBlock::fromPayload(p);
    _log << AllocatorLog::Deallocate(block->size);
    _upstream->deallocate(block);
  }

  void* reallocate(void* p, size_t n) override {
    auto block = AllocatedBlock::fromPayload(p);
    auto oldSize = block->size;
    block = reinterpret_cast<AllocatedBlock*>(
        _upstream->reallocate(block, sizeof(AllocatedBlock) + n - 1));
    if (block) {
      _log << AllocatorLog::Reallocate(oldSize, n);
      ARDUINOJSON_ASSERT(block->size == oldSize);
      block->size = n;
      return block->payload;
    } else {
      _log << AllocatorLog::ReallocateFail(oldSize, n);
      return nullptr;
    }
  }

  void clearLog() {
    _log = AllocatorLog();
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
  Allocator* _upstream;
};

class ControllableAllocator : public ArduinoJson::Allocator {
 public:
  ControllableAllocator(
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : _enabled(true), _upstream(upstream) {}
  virtual ~ControllableAllocator() {}

  void* allocate(size_t n) override {
    return _enabled ? _upstream->allocate(n) : 0;
  }

  void deallocate(void* p) override {
    _upstream->deallocate(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    return _upstream->reallocate(ptr, n);
  }

  void disable() {
    _enabled = false;
  }

 private:
  bool _enabled;
  Allocator* _upstream;
};

class TimebombAllocator : public ArduinoJson::Allocator {
 public:
  TimebombAllocator(
      size_t initialCountdown,
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : _countdown(initialCountdown), _upstream(upstream) {}
  virtual ~TimebombAllocator() {}

  void* allocate(size_t n) override {
    if (!_countdown)
      return nullptr;
    _countdown--;
    return _upstream->allocate(n);
  }

  void deallocate(void* p) override {
    _upstream->deallocate(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    if (!_countdown)
      return nullptr;
    _countdown--;
    return _upstream->reallocate(ptr, n);
  }

  void setCountdown(size_t value) {
    _countdown = value;
  }

 private:
  size_t _countdown = 0;
  Allocator* _upstream;
};
