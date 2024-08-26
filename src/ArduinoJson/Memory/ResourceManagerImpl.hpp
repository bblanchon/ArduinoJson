// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline Slot<VariantData> ResourceManager::allocVariant() {
  auto p = variantPools_.allocSlot(allocator_);
  if (!p) {
    overflowed_ = true;
    return {};
  }
  return {new (p.ptr()) VariantData, p.id()};
}

inline void ResourceManager::freeVariant(Slot<VariantData> variant) {
  variant->clear(this);
  variantPools_.freeSlot(variant);
}

inline VariantData* ResourceManager::getVariant(SlotId id) const {
  return reinterpret_cast<VariantData*>(variantPools_.getSlot(id));
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
