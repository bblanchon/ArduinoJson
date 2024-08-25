// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>

#include <sstream>

namespace {

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

class AllocatorLogEntry {
 public:
  AllocatorLogEntry(std::string s, size_t n = 1) : str_(s), count_(n) {}

  const std::string& str() const {
    return str_;
  }

  size_t count() const {
    return count_;
  }

  AllocatorLogEntry operator*(size_t n) const {
    return AllocatorLogEntry(str_, n);
  }

 private:
  std::string str_;
  size_t count_;
};

inline AllocatorLogEntry Allocate(size_t s) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "allocate(%zu)", s);
  return AllocatorLogEntry(buffer);
}

inline AllocatorLogEntry AllocateFail(size_t s) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "allocate(%zu) -> nullptr", s);
  return AllocatorLogEntry(buffer);
}

inline AllocatorLogEntry Reallocate(size_t s1, size_t s2) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "reallocate(%zu, %zu)", s1, s2);
  return AllocatorLogEntry(buffer);
}

inline AllocatorLogEntry ReallocateFail(size_t s1, size_t s2) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "reallocate(%zu, %zu) -> nullptr", s1, s2);
  return AllocatorLogEntry(buffer);
}

inline AllocatorLogEntry Deallocate(size_t s) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "deallocate(%zu)", s);
  return AllocatorLogEntry(buffer);
}

class AllocatorLog {
 public:
  AllocatorLog() = default;
  AllocatorLog(std::initializer_list<AllocatorLogEntry> list) {
    for (auto& entry : list)
      append(entry);
  }

  void clear() {
    log_.str("");
  }

  void append(const AllocatorLogEntry& entry) {
    for (size_t i = 0; i < entry.count(); i++)
      log_ << entry.str() << "\n";
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

  size_t allocatedBytes() const {
    return allocatedBytes_;
  }

  void* allocate(size_t n) override {
    auto block = reinterpret_cast<AllocatedBlock*>(
        upstream_->allocate(sizeof(AllocatedBlock) + n - 1));
    if (block) {
      log_.append(Allocate(n));
      allocatedBytes_ += n;
      block->size = n;
      return block->payload;
    } else {
      log_.append(AllocateFail(n));
      return nullptr;
    }
  }

  void deallocate(void* p) override {
    auto block = AllocatedBlock::fromPayload(p);
    allocatedBytes_ -= block->size;
    log_.append(Deallocate(block ? block->size : 0));
    upstream_->deallocate(block);
  }

  void* reallocate(void* p, size_t n) override {
    auto block = AllocatedBlock::fromPayload(p);
    auto oldSize = block ? block->size : 0;
    block = reinterpret_cast<AllocatedBlock*>(
        upstream_->reallocate(block, sizeof(AllocatedBlock) + n - 1));
    if (block) {
      log_.append(Reallocate(oldSize, n));
      block->size = n;
      allocatedBytes_ += n - oldSize;
      return block->payload;
    } else {
      log_.append(ReallocateFail(oldSize, n));
      return nullptr;
    }
  }

  void clearLog() {
    log_.clear();
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
  size_t allocatedBytes_ = 0;
};

class KillswitchAllocator : public ArduinoJson::Allocator {
 public:
  KillswitchAllocator(
      Allocator* upstream = ArduinoJson::detail::DefaultAllocator::instance())
      : working_(true), upstream_(upstream) {}
  virtual ~KillswitchAllocator() {}

  void* allocate(size_t n) override {
    return working_ ? upstream_->allocate(n) : 0;
  }

  void deallocate(void* p) override {
    upstream_->deallocate(p);
  }

  void* reallocate(void* ptr, size_t n) override {
    return working_ ? upstream_->reallocate(ptr, n) : 0;
  }

  // Turn the killswitch on, so all allocation fail
  void on() {
    working_ = false;
  }

 private:
  bool working_;
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
}  // namespace

inline size_t sizeofPoolList(size_t n = ARDUINOJSON_INITIAL_POOL_COUNT) {
  using namespace ArduinoJson::detail;
  return sizeof(MemoryPool<VariantData>) * n;
}

inline size_t sizeofPool(
    ArduinoJson::detail::SlotCount n = ARDUINOJSON_POOL_CAPACITY) {
  using namespace ArduinoJson::detail;
  return MemoryPool<VariantData>::slotsToBytes(n);
}

inline size_t sizeofStringBuffer(size_t iteration = 1) {
  // returns 31, 63, 127, 255, etc.
  auto capacity = ArduinoJson::detail::StringBuilder::initialCapacity;
  for (size_t i = 1; i < iteration; i++)
    capacity = capacity * 2 + 1;
  return ArduinoJson::detail::sizeofString(capacity);
}

inline size_t sizeofString(const char* s) {
  return ArduinoJson::detail::sizeofString(strlen(s));
}
