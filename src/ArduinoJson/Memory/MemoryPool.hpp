// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/integer.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

using SlotId = uint_t<ARDUINOJSON_SLOT_ID_SIZE * 8>;
using SlotCount = SlotId;
const SlotId NULL_SLOT = SlotId(-1);

template <typename T>
class Slot {
 public:
  Slot() : ptr_(nullptr), id_(NULL_SLOT) {}
  Slot(T* p, SlotId id) : ptr_(p), id_(id) {
    ARDUINOJSON_ASSERT((p == nullptr) == (id == NULL_SLOT));
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

  SlotId id() const {
    return id_;
  }

  T* ptr() const {
    return ptr_;
  }

  T* operator->() const {
    ARDUINOJSON_ASSERT(ptr_ != nullptr);
    return ptr_;
  }

 private:
  T* ptr_;
  SlotId id_;
};

template <typename T>
class MemoryPool {
 public:
  void create(SlotCount cap, Allocator* allocator) {
    ARDUINOJSON_ASSERT(cap > 0);
    slots_ = reinterpret_cast<T*>(allocator->allocate(slotsToBytes(cap)));
    capacity_ = slots_ ? cap : 0;
    usage_ = 0;
  }

  void destroy(Allocator* allocator) {
    if (slots_)
      allocator->deallocate(slots_);
    slots_ = nullptr;
    capacity_ = 0;
    usage_ = 0;
  }

  Slot<T> allocSlot() {
    if (!slots_)
      return {};
    if (usage_ >= capacity_)
      return {};
    auto index = usage_++;
    return {slots_ + index, SlotId(index)};
  }

  T* getSlot(SlotId id) const {
    ARDUINOJSON_ASSERT(id < usage_);
    return slots_ + id;
  }

  void clear() {
    usage_ = 0;
  }

  void shrinkToFit(Allocator* allocator) {
    auto newSlots = reinterpret_cast<T*>(
        allocator->reallocate(slots_, slotsToBytes(usage_)));
    if (newSlots) {
      slots_ = newSlots;
      capacity_ = usage_;
    }
  }

  SlotCount usage() const {
    return usage_;
  }

  static SlotCount bytesToSlots(size_t n) {
    return static_cast<SlotCount>(n / sizeof(T));
  }

  static size_t slotsToBytes(SlotCount n) {
    return n * sizeof(T);
  }

 private:
  SlotCount capacity_;
  SlotCount usage_;
  T* slots_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
