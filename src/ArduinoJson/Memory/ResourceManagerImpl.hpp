// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Polyfills/alias_cast.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline Slot<VariantData> ResourceManager::allocVariant() {
  auto p = variantPools_.allocSlot(allocator_);
  if (!p) {
    overflowed_ = true;
    return {};
  }
  return {new (&p->variant) VariantData, p.id()};
}

inline void ResourceManager::freeVariant(Slot<VariantData> variant) {
  variant->clear(this);
  variantPools_.freeSlot({alias_cast<SlotData*>(variant.ptr()), variant.id()});
}

inline VariantData* ResourceManager::getVariant(SlotId id) const {
  return reinterpret_cast<VariantData*>(variantPools_.getSlot(id));
}

#if ARDUINOJSON_USE_EXTENSIONS
inline Slot<VariantExtension> ResourceManager::allocExtension() {
  auto p = variantPools_.allocSlot(allocator_);
  if (!p) {
    overflowed_ = true;
    return {};
  }
  return {&p->extension, p.id()};
}

inline void ResourceManager::freeExtension(SlotId id) {
  auto p = getExtension(id);
  variantPools_.freeSlot({reinterpret_cast<SlotData*>(p), id});
}

inline VariantExtension* ResourceManager::getExtension(SlotId id) const {
  return &variantPools_.getSlot(id)->extension;
}
#endif

ARDUINOJSON_END_PRIVATE_NAMESPACE
