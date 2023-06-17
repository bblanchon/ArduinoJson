// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/VariantPool.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void VariantPool::create(size_t cap, Allocator* allocator) {
  ARDUINOJSON_ASSERT(data_ == nullptr);
  if (!cap)
    return;
  data_ = reinterpret_cast<char*>(allocator->allocate(cap));
  if (data_) {
    capacity_ = cap;
    usage_ = 0;
  }
}

inline void VariantPool::destroy(Allocator* allocator) {
  if (data_)
    allocator->deallocate(data_);
  data_ = nullptr;
  capacity_ = 0;
  usage_ = 0;
}

inline ptrdiff_t VariantPool::shrinkToFit(Allocator* allocator) {
  auto originalPool = data_;
  data_ = reinterpret_cast<char*>(allocator->reallocate(data_, usage_));
  if (data_)
    capacity_ = usage_;
  return data_ - originalPool;
}

inline VariantSlot* VariantPool::allocVariant() {
  if (!data_)
    return nullptr;
  if (usage_ + sizeof(VariantSlot) > capacity_)
    return nullptr;
  auto p = data_ + usage_;
  usage_ += sizeof(VariantSlot);
  return new (p) VariantSlot;
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

ARDUINOJSON_END_PRIVATE_NAMESPACE
