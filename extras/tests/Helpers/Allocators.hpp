// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/VariantPool.hpp>

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
    log_ << s << "\n";
    return *this;
  }

  std::string str() const {
    auto s = log_.str();
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
  std::ostringstream log_;
};

class SpyingAllocator : public ArduinoJson::Allocator {
 public:
  SpyingAllocator(
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : upstream_(upstream) {}
  virtual ~SpyingAllocator() {}

  void* allocate(size_t n) override {
    auto block = reinterpret_cast<AllocatedBlock*>(
        upstream_->allocate(sizeof(AllocatedBlock) + n - 1));
    if (block) {
      log_ << AllocatorLog::Allocate(n);
      block->size = n;
      return block->payload;
    } else {
      log_ << AllocatorLog::AllocateFail(n);
      return nullptr;
    }
  }

  void deallocate(void* p) override {
    auto block = AllocatedBlock::fromPayload(p);
    log_ << AllocatorLog::Deallocate(block ? block->size : 0);
    upstream_->deallocate(block);
  }

  void* reallocate(void* p, size_t n) override {
    auto block = AllocatedBlock::fromPayload(p);
    auto oldSize = block ? block->size : 0;
    block = reinterpret_cast<AllocatedBlock*>(
        upstream_->reallocate(block, sizeof(AllocatedBlock) + n - 1));
    if (block) {
      log_ << AllocatorLog::Reallocate(oldSize, n);
      block->size = n;
      return block->payload;
    } else {
      log_ << AllocatorLog::ReallocateFail(oldSize, n);
      return nullptr;
    }
  }

  void clearLog() {
    log_ = AllocatorLog();
  }

  const AllocatorLog& log() const {
    return log_;
  }

 private:
  struct AllocatedBlock {
    size_t size;
    char payload[1];

    static AllocatedBlock* fromPayload(void* p) {
      if (!p)
        return nullptr;
      return reinterpret_cast<AllocatedBlock*>(
          // Cast to void* to silence "cast increases required alignment of
          // target type [-Werror=cast-align]"
          reinterpret_cast<void*>(reinterpret_cast<char*>(p) -
                                  offsetof(AllocatedBlock, payload)));
    }
  };

  AllocatorLog log_;
  Allocator* upstream_;
};

class ControllableAllocator : public ArduinoJson::Allocator {
 public:
  ControllableAllocator(
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : enabled_(true), upstream_(upstream) {}
  virtual ~ControllableAllocator() {}

  void* allocate(size_t n) override {
    return enabled_ ? upstream_->allocate(n) : 0;
  }

  void deallocate(void* p) override {
    upstream_->deallocate(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    return enabled_ ? upstream_->reallocate(ptr, n) : 0;
  }

  void disable() {
    enabled_ = false;
  }

 private:
  bool enabled_;
  Allocator* upstream_;
};

class TimebombAllocator : public ArduinoJson::Allocator {
 public:
  TimebombAllocator(
      size_t initialCountdown,
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : countdown_(initialCountdown), upstream_(upstream) {}
  virtual ~TimebombAllocator() {}

  void* allocate(size_t n) override {
    if (!countdown_)
      return nullptr;
    countdown_--;
    return upstream_->allocate(n);
  }

  void deallocate(void* p) override {
    upstream_->deallocate(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    if (!countdown_)
      return nullptr;
    countdown_--;
    return upstream_->reallocate(ptr, n);
  }

  void setCountdown(size_t value) {
    countdown_ = value;
  }

 private:
  size_t countdown_ = 0;
  Allocator* upstream_;
};

inline size_t sizeofPoolList(size_t n = ARDUINOJSON_INITIAL_POOL_COUNT) {
  return sizeof(ArduinoJson::detail::VariantPool) * n;
}

inline size_t sizeofPool(
    ArduinoJson::detail::SlotCount n = ARDUINOJSON_POOL_CAPACITY) {
  return ArduinoJson::detail::VariantPool::slotsToBytes(n);
}
