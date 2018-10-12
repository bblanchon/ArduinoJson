// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Strings/StringTypes.hpp"
#include "JsonVariantData.hpp"
#include "Slot.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline bool slotSetKey(Slot* slot, TKey key, MemoryPool* pool) {
  const char* dup = key.save(pool);
  if (!dup) return false;
  slot->key = dup;
  slot->value.keyIsStatic = false;
  return true;
}

inline bool slotSetKey(Slot* slot, ZeroTerminatedRamStringConst key,
                       MemoryPool* pool) {
  slot->key = key.save(pool);
  slot->value.keyIsStatic = true;
  return true;
}

inline bool slotSetKey(Slot* slot, StringInMemoryPool key, MemoryPool* pool) {
  slot->key = key.save(pool);
  slot->value.keyIsStatic = false;
  return true;
}

inline const Slot* slotAdvance(const Slot* slot, size_t distance) {
  while (distance && slot) {
    slot = slot->next;
    distance--;
  }
  return slot;
}

inline Slot* slotAdvance(Slot* slot, size_t distance) {
  while (distance && slot) {
    slot = slot->next;
    distance--;
  }
  return slot;
}

inline size_t slotSize(const Slot* slot) {
  size_t n = 0;
  while (slot) {
    n++;
    slot = slot->next;
  }
  return n;
}
}  // namespace ARDUINOJSON_NAMESPACE
