// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void ResourceManager::freeSlot(SlotWithId slot) {
  if (slot->ownsKey())
    dereferenceString(slot->key());
  slot->data()->setNull(this);
  variantPools_.freeSlot(slot);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
