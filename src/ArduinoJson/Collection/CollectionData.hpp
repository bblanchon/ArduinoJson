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

class CollectionData {
  VariantSlot* _head;
  VariantSlot* _tail;

 public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance

  // Array only

  VariantData* getElement(size_t index) const;

  // Object only

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const;

  template <typename TAdaptedString>
  bool containsKey(const TAdaptedString& key) const;

  // Generic

  void clear();
  size_t memoryUsage() const;
  size_t size() const;

  void addSlot(VariantSlot*);
  void removeSlot(VariantSlot* slot);

  VariantSlot* head() const {
    return _head;
  }

  void movePointers(ptrdiff_t variantDistance);

  VariantSlot* getSlot(size_t index) const;

  template <typename TAdaptedString>
  VariantSlot* getSlot(TAdaptedString key) const;

 private:
  VariantSlot* getPreviousSlot(VariantSlot*) const;
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
