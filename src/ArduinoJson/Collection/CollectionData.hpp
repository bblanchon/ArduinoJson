// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

#include <stddef.h>  // size_t

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantData;
class ResourceManager;

class CollectionIterator {
  friend class CollectionData;

 public:
  CollectionIterator() : slot_(nullptr), currentId_(NULL_SLOT) {}

  void next(const ResourceManager* resources);

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
    return reinterpret_cast<VariantData*>(slot_);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(slot_);
  }

 private:
  CollectionIterator(VariantData* slot, SlotId slotId);

  VariantData* slot_;
  SlotId currentId_, nextId_;
};

class CollectionData {
  SlotId head_ = NULL_SLOT;
  SlotId tail_ = NULL_SLOT;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  using iterator = CollectionIterator;

  iterator createIterator(const ResourceManager* resources) const;

  size_t size(const ResourceManager*) const;
  size_t nesting(const ResourceManager*) const;

  void clear(ResourceManager* resources);

  static void clear(CollectionData* collection, ResourceManager* resources) {
    if (!collection)
      return;
    collection->clear(resources);
  }

  SlotId head() const {
    return head_;
  }

 protected:
  void appendOne(Slot<VariantData> slot, const ResourceManager* resources);
  void appendPair(Slot<VariantData> key, Slot<VariantData> value,
                  const ResourceManager* resources);

  void removeOne(iterator it, ResourceManager* resources);
  void removePair(iterator it, ResourceManager* resources);

 private:
  Slot<VariantData> getPreviousSlot(VariantData*, const ResourceManager*) const;
};

inline const VariantData* collectionToVariant(
    const CollectionData* collection) {
  const void* data = collection;  // prevent warning cast-align
  return reinterpret_cast<const VariantData*>(data);
}

inline VariantData* collectionToVariant(CollectionData* collection) {
  void* data = collection;  // prevent warning cast-align
  return reinterpret_cast<VariantData*>(data);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
