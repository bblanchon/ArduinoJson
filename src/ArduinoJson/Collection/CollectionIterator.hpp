// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

#include <stddef.h>  // size_t

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct VariantData;
class ResourceManager;

class CollectionIterator {
  friend class VariantImpl;

 public:
  CollectionIterator() : slot_(nullptr), currentId_(NULL_SLOT) {}

  void move(const ResourceManager* resources);

  bool done() const {
    return slot_ == nullptr;
  }

  bool operator==(const CollectionIterator& other) const {
    return slot_ == other.slot_;
  }

  bool operator!=(const CollectionIterator& other) const {
    return slot_ != other.slot_;
  }

  VariantData* operator->() {
    ARDUINOJSON_ASSERT(slot_ != nullptr);
    return data();
  }

  VariantData& operator*() {
    ARDUINOJSON_ASSERT(slot_ != nullptr);
    return *data();
  }

  const VariantData& operator*() const {
    ARDUINOJSON_ASSERT(slot_ != nullptr);
    return *data();
  }

  VariantData* data() {
    return slot_;
  }

  const VariantData* data() const {
    return slot_;
  }

 private:
  CollectionIterator(VariantData* slot, SlotId slotId)
      : slot_(slot), currentId_(slotId) {}

  VariantData* slot_;
  SlotId currentId_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
