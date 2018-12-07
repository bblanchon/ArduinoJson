// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Polyfills/assert.hpp"
#include "../Strings/StringWrappers.hpp"
#include "VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline bool slotSetKey(VariantSlot* var, TKey key, MemoryPool* pool) {
  if (!var) return false;
  char* dup = key.save(pool);
  if (!dup) return false;
  var->setOwnedKey(dup);
  return true;
}

inline bool slotSetKey(VariantSlot* var, ConstRamStringWrapper key,
                       MemoryPool*) {
  if (!var) return false;
  var->setLinkedKey(key.c_str());
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
