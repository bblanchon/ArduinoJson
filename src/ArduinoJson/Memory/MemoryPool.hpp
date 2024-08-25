// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/integer.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantData;
using SlotId = uint_t<ARDUINOJSON_SLOT_ID_SIZE * 8>;
using SlotCount = SlotId;
const SlotId NULL_SLOT = SlotId(-1);

class SlotWithId {
 public:
  SlotWithId() : slot_(nullptr), id_(NULL_SLOT) {}
  SlotWithId(VariantData* slot, SlotId id) : slot_(slot), id_(id) {
    ARDUINOJSON_ASSERT((slot == nullptr) == (id == NULL_SLOT));
  }

  explicit operator bool() const {
    return slot_ != nullptr;
  }

  SlotId id() const {
    return id_;
  }

  VariantData* slot() const {
    return slot_;
  }

  VariantData* operator->() {
    ARDUINOJSON_ASSERT(slot_ != nullptr);
    return slot_;
  }

 private:
  VariantData* slot_;
  SlotId id_;
};

class MemoryPool {
 public:
  void create(SlotCount cap, Allocator* allocator);
  void destroy(Allocator* allocator);

  SlotWithId allocSlot();
  VariantData* getSlot(SlotId id) const;
  void clear();
  void shrinkToFit(Allocator*);
  SlotCount usage() const;

  static SlotCount bytesToSlots(size_t);
  static size_t slotsToBytes(SlotCount);

 private:
  SlotCount capacity_;
  SlotCount usage_;
  VariantData* slots_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
