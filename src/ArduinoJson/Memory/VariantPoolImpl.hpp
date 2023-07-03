// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void VariantPool::create(size_t cap, Allocator* allocator) {
  ARDUINOJSON_ASSERT(slots_ == nullptr);
  if (!cap)
    return;
  slots_ = reinterpret_cast<VariantSlot*>(allocator->allocate(cap));
  if (slots_) {
    capacity_ = bytesToSlots(cap);
    usage_ = 0;
  }
}

inline void VariantPool::destroy(Allocator* allocator) {
  if (slots_)
    allocator->deallocate(slots_);
  slots_ = nullptr;
  capacity_ = 0;
  usage_ = 0;
}

inline ptrdiff_t VariantPool::shrinkToFit(Allocator* allocator) {
  auto originalPool = slots_;
  slots_ = reinterpret_cast<VariantSlot*>(
      allocator->reallocate(slots_, slotsToBytes(usage_)));
  if (slots_)
    capacity_ = usage_;
  return reinterpret_cast<char*>(slots_) -
         reinterpret_cast<char*>(originalPool);
}

inline VariantSlot* VariantPool::allocVariant() {
  if (!slots_)
    return nullptr;
  if (usage_ + 1 > capacity_)
    return nullptr;
  return new (&slots_[usage_++]) VariantSlot;
}

inline size_t VariantPool::usage() const {
  return usage_;
}

inline size_t VariantPool::capacity() const {
  return capacity_;
}

inline void VariantPool::clear() {
  usage_ = 0;
}

inline size_t VariantPool::bytesToSlots(size_t n) {
  return n / sizeof(VariantSlot);
}

inline size_t VariantPool::slotsToBytes(size_t n) {
  return n * sizeof(VariantSlot);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
