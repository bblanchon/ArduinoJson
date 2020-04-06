// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool) {
  if (!var)
    return false;
  return slotSetKey(var, key, pool, typename TAdaptedString::storage_policy());
}

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool,
                       storage_policy::decide_at_runtime) {
  if (key.isStatic()) {
    return slotSetKey(var, key, pool, storage_policy::store_by_address());
  } else {
    return slotSetKey(var, key, pool, storage_policy::store_by_copy());
  }
}

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool*,
                       storage_policy::store_by_address) {
  ARDUINOJSON_ASSERT(var);
  var->setLinkedKey(make_not_null(key.data()));
  return true;
}

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool,
                       storage_policy::store_by_copy) {
  const char* dup = key.save(pool);
  if (!dup)
    return false;
  ARDUINOJSON_ASSERT(var);
  var->setOwnedKey(make_not_null(dup));
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

inline VariantData* slotData(VariantSlot* slot) {
  return reinterpret_cast<VariantData*>(slot);
}
}  // namespace ARDUINOJSON_NAMESPACE
