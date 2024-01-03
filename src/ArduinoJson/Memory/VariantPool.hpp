// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/integer.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantSlot;
using SlotId = uint_t<ARDUINOJSON_SLOT_ID_SIZE * 8>::type;
using SlotCount = SlotId;
const SlotId NULL_SLOT = SlotId(-1);

class SlotWithId {
 public:
  SlotWithId() : slot_(nullptr), id_(NULL_SLOT) {}
  SlotWithId(VariantSlot* slot, SlotId id) : slot_(slot), id_(id) {
    ARDUINOJSON_ASSERT((slot == nullptr) == (id == NULL_SLOT));
  }

  SlotId id() const {
    return id_;
  }

  operator VariantSlot*() {
    return slot_;
  }

  VariantSlot* operator->() {
    ARDUINOJSON_ASSERT(slot_ != nullptr);
    return slot_;
  }

 private:
  VariantSlot* slot_;
  SlotId id_;
};

class VariantPool {
 public:
  void create(SlotCount cap, Allocator* allocator);
  void destroy(Allocator* allocator);

  SlotWithId allocSlot();
  VariantSlot* getSlot(SlotId id) const;
  void clear();
  void shrinkToFit(Allocator*);
  SlotCount usage() const;

  static SlotCount bytesToSlots(size_t);
  static size_t slotsToBytes(SlotCount);

 private:
  SlotCount capacity_;
  SlotCount usage_;
  VariantSlot* slots_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
