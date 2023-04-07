// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct SlotKeySetter {
  SlotKeySetter(VariantSlot* instance) : instance_(instance) {}

  template <typename TStoredString>
  void operator()(TStoredString s) {
    if (!s)
      return;
    ARDUINOJSON_ASSERT(instance_ != 0);
    instance_->setKey(s);
  }

  VariantSlot* instance_;
};

template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool) {
  if (!var)
    return false;
  return storeString(pool, key, SlotKeySetter(var));
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
ARDUINOJSON_END_PRIVATE_NAMESPACE
