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
  VariantSlot* head_;
  VariantSlot* tail_;

 public:
  void clear();
  size_t memoryUsage() const;
  size_t size() const;

  void add(VariantSlot*);
  void remove(VariantSlot* slot);

  VariantSlot* head() const {
    return head_;
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

bool arrayEquals(const detail::CollectionData& lhs,
                 const detail::CollectionData& rhs);
bool arrayEquals(const detail::CollectionData* lhs,
                 const detail::CollectionData* rhs);
bool objectEquals(const detail::CollectionData& lhs,
                  const detail::CollectionData& rhs);

ARDUINOJSON_END_PRIVATE_NAMESPACE
