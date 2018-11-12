// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Polyfills/assert.hpp"
#include "../Strings/StringTypes.hpp"
#include "JsonVariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline bool slotSetKey(VariantSlot* var, TKey key, MemoryPool* pool) {
  StringSlot* slot = key.save(pool);
  if (!slot) return false;
  var->ownedKey = slot;
  var->value.keyIsOwned = true;
  return true;
}

inline bool slotSetKey(VariantSlot* var, ZeroTerminatedRamStringConst key,
                       MemoryPool*) {
  var->linkedKey = key.c_str();
  var->value.keyIsOwned = false;
  return true;
}

inline bool slotSetKey(VariantSlot* var, StringInMemoryPool key, MemoryPool*) {
  var->ownedKey = key.slot();
  var->value.keyIsOwned = true;
  return true;
}

inline const char* slotGetKey(const VariantSlot* var) {
  return var->value.keyIsOwned ? var->ownedKey->value : var->linkedKey;
}

inline const VariantSlot* slotAdvance(const VariantSlot* var, size_t distance) {
  while (distance && var) {
    var = var->next;
    distance--;
  }
  return var;
}

inline VariantSlot* slotAdvance(VariantSlot* var, size_t distance) {
  while (distance && var) {
    var = var->next;
    distance--;
  }
  return var;
}

inline size_t slotSize(const VariantSlot* var) {
  size_t n = 0;
  while (var) {
    n++;
    var = var->next;
  }
  return n;
}

void variantFree(JsonVariantData* var, MemoryPool* pool);

inline void slotFree(VariantSlot* var, MemoryPool* pool) {
  ARDUINOJSON_ASSERT(var != 0);
  ARDUINOJSON_ASSERT(pool != 0);

  variantFree(&var->value, pool);

  if (var->value.keyIsOwned) pool->freeString(var->ownedKey);

  pool->freeVariant(var);
}
}  // namespace ARDUINOJSON_NAMESPACE
