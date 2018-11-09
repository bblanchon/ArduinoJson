// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "JsonVariantData.hpp"
#include "SlotFunctions.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline VariantSlot* objectFindSlot(const JsonObjectData* obj, TKey key) {
  if (!obj) return 0;
  VariantSlot* slot = obj->head;
  while (slot) {
    if (key.equals(slotGetKey(slot))) break;
    slot = slot->next;
  }
  return slot;
}

template <typename TKey>
inline bool objectContainsKey(const JsonObjectData* obj, const TKey& key) {
  return objectFindSlot(obj, key) != 0;
}

template <typename TKey>
inline JsonVariantData* objectAdd(JsonObjectData* obj, TKey key,
                                  MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot) return 0;

  slot->next = 0;
  slot->value.type = JSON_NULL;

  if (obj->tail) {
    slot->prev = obj->tail;
    obj->tail->next = slot;
    obj->tail = slot;
  } else {
    slot->prev = 0;
    obj->head = slot;
    obj->tail = slot;
  }

  if (!slotSetKey(slot, key, pool)) return 0;
  return &slot->value;
}

template <typename TKey>
inline JsonVariantData* objectSet(JsonObjectData* obj, TKey key,
                                  MemoryPool* pool) {
  if (!obj) return 0;

  // ignore null key
  if (key.isNull()) return 0;

  // search a matching key
  VariantSlot* slot = objectFindSlot(obj, key);
  if (slot) return &slot->value;

  return objectAdd(obj, key, pool);
}

template <typename TKey>
inline JsonVariantData* objectGet(const JsonObjectData* obj, TKey key) {
  VariantSlot* slot = objectFindSlot(obj, key);
  return slot ? &slot->value : 0;
}

inline void objectClear(JsonObjectData* obj) {
  if (!obj) return;
  obj->head = 0;
  obj->tail = 0;
}

inline void objectRemove(JsonObjectData* obj, VariantSlot* slot,
                         MemoryPool* pool) {
  if (!obj) return;
  if (!slot) return;
  if (slot->prev)
    slot->prev->next = slot->next;
  else
    obj->head = slot->next;
  if (slot->next)
    slot->next->prev = slot->prev;
  else
    obj->tail = slot->prev;

  slotFree(slot, pool);
}

inline size_t objectSize(const JsonObjectData* obj) {
  if (!obj) return 0;
  return slotSize(obj->head);
}

// bool variantCopy(JsonVariantData*, const JsonVariantData*, MemoryPool*);

inline bool objectCopy(JsonObjectData* dst, const JsonObjectData* src,
                       MemoryPool* pool) {
  if (!dst || !src) return false;
  objectClear(dst);
  for (VariantSlot* s = src->head; s; s = s->next) {
    JsonVariantData* var;
    if (s->value.keyIsOwned)
      var = objectAdd(dst, ZeroTerminatedRamString(s->ownedKey->value), pool);
    else
      var = objectAdd(dst, ZeroTerminatedRamStringConst(s->linkedKey), pool);
    if (!variantCopy(var, &s->value, pool)) return false;
  }
  return true;
}

inline bool objectEquals(const JsonObjectData* o1, const JsonObjectData* o2) {
  if (o1 == o2) return true;
  if (!o1 || !o2) return false;

  for (VariantSlot* s = o1->head; s; s = s->next) {
    JsonVariantData* v1 = &s->value;
    JsonVariantData* v2 = objectGet(o2, makeString(slotGetKey(s)));
    if (!variantEquals(v1, v2)) return false;
  }
  return true;
}
}  // namespace ARDUINOJSON_NAMESPACE
