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
  void clear();
  size_t memoryUsage() const;
  size_t size() const;

  void add(VariantSlot*);
  void remove(VariantSlot* slot);

  VariantSlot* head() const {
    return _head;
  }

  void movePointers(ptrdiff_t variantDistance);

  VariantSlot* get(size_t index) const;

  template <typename TAdaptedString>
  VariantSlot* get(TAdaptedString key) const;

 private:
  VariantSlot* getPrevious(VariantSlot*) const;
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
