// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void MemoryPool::create(SlotCount cap, Allocator* allocator) {
  ARDUINOJSON_ASSERT(cap > 0);
  slots_ =
      reinterpret_cast<VariantSlot*>(allocator->allocate(slotsToBytes(cap)));
  capacity_ = slots_ ? cap : 0;
  usage_ = 0;
}

inline void MemoryPool::destroy(Allocator* allocator) {
  if (slots_)
    allocator->deallocate(slots_);
  slots_ = nullptr;
  capacity_ = 0;
  usage_ = 0;
}

inline void MemoryPool::shrinkToFit(Allocator* allocator) {
  auto newSlots = reinterpret_cast<VariantSlot*>(
      allocator->reallocate(slots_, slotsToBytes(usage_)));
  if (newSlots) {
    slots_ = newSlots;
    capacity_ = usage_;
  }
}

inline SlotWithId MemoryPool::allocSlot() {
  if (!slots_)
    return {};
  if (usage_ >= capacity_)
    return {};
  auto index = usage_++;
  auto slot = &slots_[index];
  return {slot, SlotId(index)};
}

inline VariantSlot* MemoryPool::getSlot(SlotId id) const {
  ARDUINOJSON_ASSERT(id < usage_);
  return &slots_[id];
}

inline SlotCount MemoryPool::usage() const {
  return usage_;
}

inline void MemoryPool::clear() {
  usage_ = 0;
}

inline SlotCount MemoryPool::bytesToSlots(size_t n) {
  return static_cast<SlotCount>(n / sizeof(VariantSlot));
}

inline size_t MemoryPool::slotsToBytes(SlotCount n) {
  return n * sizeof(VariantSlot);
}

inline SlotWithId MemoryPoolList::allocFromFreeList() {
  ARDUINOJSON_ASSERT(freeList_ != NULL_SLOT);
  auto id = freeList_;
  auto slot = getSlot(freeList_);
  freeList_ = slot->free.next;
  return {slot, id};
}

inline void MemoryPoolList::freeSlot(SlotWithId slot) {
  slot->free.next = freeList_;
  freeList_ = slot.id();
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
