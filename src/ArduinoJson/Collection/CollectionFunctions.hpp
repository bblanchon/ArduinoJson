// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void collectionClear(CollectionData* c, ResourceManager* resources) {
  if (!c)
    return;
  c->clear(resources);
}

inline void collectionRemove(CollectionData* data, VariantSlot* slot,
                             ResourceManager* resources) {
  if (!data)
    return;
  data->removeSlot(slot, resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
