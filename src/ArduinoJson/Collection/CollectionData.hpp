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
  VariantSlot* head_ = 0;
  VariantSlot* tail_ = 0;

 public:
  // Placement new
  static void* operator new(size_t, void* p) noexcept {
    return p;
  }

  static void operator delete(void*, void*) noexcept {}

  size_t memoryUsage() const;
  size_t size() const;

  VariantData* addMember(StringNode* key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* addMember(TAdaptedString key, ResourceManager* resources);

  void clear(ResourceManager* resources);
  bool copyFrom(const CollectionData& src, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const;

  void removeSlot(VariantSlot* slot, ResourceManager* resources);

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key, ResourceManager* resources);

  VariantSlot* head() const {
    return head_;
  }

  void movePointers(ptrdiff_t variantDistance);

 protected:
  void addSlot(VariantSlot*);

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

bool objectEquals(const detail::CollectionData& lhs,
                  const detail::CollectionData& rhs);
bool objectEquals(const detail::CollectionData* lhs,
                  const detail::CollectionData* rhs);

ARDUINOJSON_END_PRIVATE_NAMESPACE
