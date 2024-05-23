// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

#include <stddef.h>  // size_t

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class VariantData;
class VariantSlot;

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

  const char* key() const;
  bool ownsKey() const;

  void setKey(StringNode*);
  void setKey(const char*);

  VariantData* data() {
    return reinterpret_cast<VariantData*>(slot_);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(slot_);
  }

 private:
  CollectionIterator(VariantSlot* slot, SlotId slotId);

  VariantSlot* slot_;
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

  iterator createIterator(const ResourceManager* resources) const {
    return iterator(resources->getSlot(head_), head_);
  }

  size_t size(const ResourceManager*) const;
  size_t nesting(const ResourceManager*) const;

  void clear(ResourceManager* resources);

  static void clear(CollectionData* collection, ResourceManager* resources) {
    if (!collection)
      return;
    collection->clear(resources);
  }

  void remove(iterator it, ResourceManager* resources);

  static void remove(CollectionData* collection, iterator it,
                     ResourceManager* resources) {
    if (collection)
      return collection->remove(it, resources);
  }

  SlotId head() const {
    return head_;
  }

  void addSlot(SlotWithId slot, ResourceManager* resources);

 protected:
  iterator addSlot(ResourceManager*);

 private:
  SlotWithId getPreviousSlot(VariantSlot*, const ResourceManager*) const;
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
