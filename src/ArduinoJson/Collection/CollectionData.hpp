// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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
  CollectionIterator() : slot_(nullptr) {}

  void next();

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

  VariantData* data() {
    return reinterpret_cast<VariantData*>(slot_);
  }

  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(slot_);
  }

 private:
  CollectionIterator(VariantSlot* slot) : slot_(slot) {}

  VariantSlot* slot_;
};

class CollectionData {
  VariantSlot* head_ = 0;
  VariantSlot* tail_ = 0;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  using iterator = CollectionIterator;

  iterator createIterator() const {
    return iterator(head_);
  }

  size_t memoryUsage() const;
  size_t size() const;
  size_t nesting() const;

  void clear(ResourceManager* resources);

  static void clear(CollectionData* collection, ResourceManager* resources) {
    if (!collection)
      return;
    collection->clear(resources);
  }

  void movePointers(ptrdiff_t variantDistance);

  void remove(iterator it, ResourceManager* resources);

  static void remove(CollectionData* collection, iterator it,
                     ResourceManager* resources) {
    if (collection)
      return collection->remove(it, resources);
  }

 protected:
  VariantSlot* addSlot(ResourceManager*);

 private:
  VariantSlot* getPreviousSlot(VariantSlot*) const;
  static void releaseSlot(VariantSlot*, ResourceManager*);
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
