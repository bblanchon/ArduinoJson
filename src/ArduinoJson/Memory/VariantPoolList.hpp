// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

using PoolCount = SlotId;

class VariantPoolList {
 public:
  VariantPoolList() = default;

  ~VariantPoolList() {
    ARDUINOJSON_ASSERT(count_ == 0);
  }

  VariantPoolList& operator=(VariantPoolList&& src) {
    ARDUINOJSON_ASSERT(count_ == 0);
    if (src.pools_ == src.preallocatedPools_) {
      memcpy(preallocatedPools_, src.preallocatedPools_,
             sizeof(preallocatedPools_));
      pools_ = preallocatedPools_;
    } else {
      pools_ = src.pools_;
      src.pools_ = nullptr;
    }
    count_ = src.count_;
    capacity_ = src.capacity_;
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
    if (count_) {
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
    if (id == NULL_SLOT)
      return nullptr;
    auto poolIndex = SlotId(id / ARDUINOJSON_POOL_CAPACITY);
    auto indexInPool = SlotId(id % ARDUINOJSON_POOL_CAPACITY);
    ARDUINOJSON_ASSERT(poolIndex < count_);
    return pools_[poolIndex].getSlot(indexInPool);
  }

  void clear(Allocator* allocator) {
    for (PoolCount i = 0; i < count_; i++)
      pools_[i].destroy(allocator);
    count_ = 0;
    if (pools_ != preallocatedPools_) {
      allocator->deallocate(pools_);
      pools_ = preallocatedPools_;
      capacity_ = ARDUINOJSON_INITIAL_POOL_COUNT;
    }
  }

  SlotCount usage() const {
    SlotCount total = 0;
    for (PoolCount i = 0; i < count_; i++)
      total = SlotCount(total + pools_[i].usage());
    return total;
  }

  void shrinkToFit(Allocator* allocator) {
    if (count_ > 0)
      pools_[count_ - 1].shrinkToFit(allocator);
    if (pools_ != preallocatedPools_ && count_ != capacity_) {
      pools_ = static_cast<VariantPool*>(
          allocator->reallocate(pools_, count_ * sizeof(VariantPool)));
      ARDUINOJSON_ASSERT(pools_ != nullptr);  // realloc to smaller can't fail
      capacity_ = count_;
    }
  }

 private:
  SlotWithId allocFromFreeList();

  SlotWithId allocFromLastPool() {
    ARDUINOJSON_ASSERT(count_ > 0);
    auto poolIndex = SlotId(count_ - 1);
    auto slot = pools_[poolIndex].allocSlot();
    if (!slot)
      return {};
    return {slot, SlotId(poolIndex * ARDUINOJSON_POOL_CAPACITY + slot.id())};
  }

  VariantPool* addPool(Allocator* allocator) {
    if (count_ == capacity_ && !increaseCapacity(allocator))
      return nullptr;
    auto pool = &pools_[count_++];
    SlotCount poolCapacity = ARDUINOJSON_POOL_CAPACITY;
    if (count_ == maxPools)  // last pool is smaller because of NULL_SLOT
      poolCapacity--;
    pool->create(poolCapacity, allocator);
    return pool;
  }

  bool increaseCapacity(Allocator* allocator) {
    if (capacity_ == maxPools)
      return false;
    void* newPools;
    auto newCapacity = PoolCount(capacity_ * 2);

    if (pools_ == preallocatedPools_) {
      newPools = allocator->allocate(newCapacity * sizeof(VariantPool));
      if (!newPools)
        return false;
      memcpy(newPools, preallocatedPools_, sizeof(preallocatedPools_));
    } else {
      newPools =
          allocator->reallocate(pools_, newCapacity * sizeof(VariantPool));
      if (!newPools)
        return false;
    }

    pools_ = static_cast<VariantPool*>(newPools);
    capacity_ = newCapacity;
    return true;
  }

  VariantPool preallocatedPools_[ARDUINOJSON_INITIAL_POOL_COUNT];
  VariantPool* pools_ = preallocatedPools_;
  PoolCount count_ = 0;
  PoolCount capacity_ = ARDUINOJSON_INITIAL_POOL_COUNT;
  SlotId freeList_ = NULL_SLOT;

 public:
  static const PoolCount maxPools =
      PoolCount(NULL_SLOT / ARDUINOJSON_POOL_CAPACITY + 1);
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
