// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantPoolList {
 public:
  VariantPoolList() = default;

  ~VariantPoolList() {
    ARDUINOJSON_ASSERT(pools_ == nullptr);
  }

  VariantPoolList& operator=(VariantPoolList&& src) {
    ARDUINOJSON_ASSERT(pools_ == nullptr);
    pools_ = src.pools_;
    count_ = src.count_;
    capacity_ = src.capacity_;
    src.pools_ = nullptr;
    src.count_ = 0;
    src.capacity_ = 0;
    return *this;
  }

  SlotWithId allocSlot(Allocator* allocator) {
    // try to allocate from free list
    if (freeList_ != NULL_SLOT) {
      return allocFromFreeList();
    }

    // try to allocate from last pool (other pools are full)
    if (pools_) {
      auto slot = allocFromLastPool();
      if (slot)
        return slot;
    }

    // create a new pool and try again
    auto pool = addPool(allocator);
    if (!pool)
      return {};

    return allocFromLastPool();
  }

  void freeSlot(SlotWithId slot);

  VariantSlot* getSlot(SlotId id) const {
    auto poolIndex = SlotId(id / ARDUINOJSON_POOL_CAPACITY);
    auto indexInPool = SlotId(id % ARDUINOJSON_POOL_CAPACITY);
    if (poolIndex >= count_)
      return nullptr;
    return pools_[poolIndex].getSlot(indexInPool);
  }

  void clear(Allocator* allocator) {
    if (!pools_)
      return;
    for (size_t i = 0; i < count_; i++)
      pools_[i].destroy(allocator);
    allocator->deallocate(pools_);
    pools_ = nullptr;
    count_ = 0;
    capacity_ = 0;
  }

  SlotCount usage() const {
    SlotCount total = 0;
    for (size_t i = 0; i < count_; i++)
      total = SlotCount(total + pools_[i].usage());
    return total;
  }

  void shrinkToFit(Allocator* allocator) {
    if (pools_)
      pools_[count_ - 1].shrinkToFit(allocator);
    if (count_ != capacity_) {
      pools_ = static_cast<VariantPool*>(
          allocator->reallocate(pools_, count_ * sizeof(VariantPool)));
      ARDUINOJSON_ASSERT(pools_ != nullptr);  // realloc to smaller can't fail
      capacity_ = count_;
    }
  }

 private:
  SlotWithId allocFromFreeList();

  SlotWithId allocFromLastPool() {
    ARDUINOJSON_ASSERT(pools_ != nullptr);
    auto poolIndex = SlotId(count_ - 1);
    auto lastPool = count_ ? &pools_[poolIndex] : nullptr;
    auto slot = lastPool->allocSlot();
    if (!slot)
      return {};
    return {slot, SlotId(poolIndex * ARDUINOJSON_POOL_CAPACITY + slot.id())};
  }

  VariantPool* addPool(Allocator* allocator) {
    if (count_ == capacity_ && !increaseCapacity(allocator))
      return nullptr;
    auto pool = &pools_[count_++];
    pool->create(ARDUINOJSON_POOL_CAPACITY, allocator);
    return pool;
  }

  bool increaseCapacity(Allocator* allocator) {
    void* newPools;
    size_t newCapacity;
    if (pools_) {
      newCapacity = capacity_ * 2;
      newPools =
          allocator->reallocate(pools_, newCapacity * sizeof(VariantPool));
    } else {
      newCapacity = ARDUINOJSON_INITIAL_POOL_COUNT;
      newPools = allocator->allocate(newCapacity * sizeof(VariantPool));
    }
    if (!newPools)
      return false;
    pools_ = static_cast<VariantPool*>(newPools);
    capacity_ = newCapacity;
    return true;
  }

  VariantPool* pools_ = nullptr;
  size_t count_ = 0;
  size_t capacity_ = 0;
  SlotId freeList_ = NULL_SLOT;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
