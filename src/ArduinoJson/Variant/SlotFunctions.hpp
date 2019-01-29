// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Polyfills/assert.hpp"
#include "../Strings/StringAdapters.hpp"
#include "VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool) {
  if (!var) return false;
  if (key.isStatic()) {
    var->setLinkedKey(key.data());
  } else {
    char* dup = key.save(pool);
    if (!dup) return false;
    var->setOwnedKey(dup);
  }
  return true;
}

inline size_t slotSize(const VariantSlot* var) {
  size_t n = 0;
  while (var) {
    n++;
    var = var->next();
  }
  return n;
}
}  // namespace ARDUINOJSON_NAMESPACE
