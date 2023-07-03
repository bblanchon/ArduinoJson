// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantSlot;

class VariantPool {
 public:
  ~VariantPool() {
    ARDUINOJSON_ASSERT(slots_ == nullptr);
  }

  VariantPool& operator=(VariantPool&& src) {
    capacity_ = src.capacity_;
    src.capacity_ = 0;
    usage_ = src.usage_;
    src.usage_ = 0;
    slots_ = src.slots_;
    src.slots_ = nullptr;
    return *this;
  }

  void create(size_t cap, Allocator* allocator);
  void destroy(Allocator* allocator);

  VariantSlot* allocVariant();
  void clear();
  ptrdiff_t shrinkToFit(Allocator*);
  size_t capacity() const;
  size_t usage() const;

  static size_t bytesToSlots(size_t);
  static size_t slotsToBytes(size_t);

 private:
  size_t capacity_ = 0;
  size_t usage_ = 0;
  VariantSlot* slots_ = nullptr;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
