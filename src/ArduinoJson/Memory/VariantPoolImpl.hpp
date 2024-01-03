// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void VariantPool::create(SlotCount cap, Allocator* allocator) {
  ARDUINOJSON_ASSERT(cap > 0);
  slots_ =
      reinterpret_cast<VariantSlot*>(allocator->allocate(slotsToBytes(cap)));
  capacity_ = slots_ ? cap : 0;
  usage_ = 0;
}

inline void VariantPool::destroy(Allocator* allocator) {
  if (slots_)
    allocator->deallocate(slots_);
  slots_ = nullptr;
  capacity_ = 0;
  usage_ = 0;
}

inline void VariantPool::shrinkToFit(Allocator* allocator) {
  auto newSlots = reinterpret_cast<VariantSlot*>(
      allocator->reallocate(slots_, slotsToBytes(usage_)));
  if (newSlots) {
    slots_ = newSlots;
    capacity_ = usage_;
  }
}

inline SlotWithId VariantPool::allocSlot() {
  if (!slots_)
    return {};
  if (usage_ >= capacity_)
    return {};
  auto index = usage_++;
  auto slot = &slots_[index];
  return {new (slot) VariantSlot, SlotId(index)};
}

inline VariantSlot* VariantPool::getSlot(SlotId id) const {
  ARDUINOJSON_ASSERT(id < usage_);
  return &slots_[id];
}

inline SlotCount VariantPool::usage() const {
  return usage_;
}

inline void VariantPool::clear() {
  usage_ = 0;
}

inline SlotCount VariantPool::bytesToSlots(size_t n) {
  return static_cast<SlotCount>(n / sizeof(VariantSlot));
}

inline size_t VariantPool::slotsToBytes(SlotCount n) {
  return n * sizeof(VariantSlot);
}

inline SlotWithId VariantPoolList::allocFromFreeList() {
  ARDUINOJSON_ASSERT(freeList_ != NULL_SLOT);
  auto id = freeList_;
  auto slot = getSlot(freeList_);
  freeList_ = slot->next();
  return {new (slot) VariantSlot, id};
}

inline void VariantPoolList::freeSlot(SlotWithId slot) {
  slot->setNext(freeList_);
  freeList_ = slot.id();
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
