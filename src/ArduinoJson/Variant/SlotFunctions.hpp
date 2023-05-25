// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline size_t slotSize(const VariantSlot* var) {
  size_t n = 0;
  while (var) {
    n++;
    var = var->next();
  }
  return n;
}

inline void slotRelease(VariantSlot* slot, MemoryPool* pool) {
  ARDUINOJSON_ASSERT(slot != nullptr);
  if (slot->ownsKey())
    pool->dereferenceString(slot->key());
  slot->data()->setNull(pool);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
