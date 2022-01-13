// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

struct SlotKeySetter {
  SlotKeySetter(VariantSlot* instance) : _instance(instance) {}

  template <typename TStoredString>
  void operator()(TStoredString s) {
    if (!s)
      return;
    ARDUINOJSON_ASSERT(_instance != 0);
    _instance->setKey(s);
  }

  VariantSlot* _instance;
};

template <typename TAdaptedString, typename TStoragePolicy>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool,
                       TStoragePolicy storage) {
  if (!var)
    return false;
  return storage.store(key, pool, SlotKeySetter(var));
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
