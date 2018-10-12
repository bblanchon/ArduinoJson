// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariantData.hpp"
#include "SlotFunctions.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline Slot* objectFindSlot(const JsonObjectData* obj, TKey key) {
  if (!obj) return 0;
  Slot* slot = obj->head;
  while (slot) {
    if (key.equals(slot->key)) break;
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
  Slot* slot = new (pool) Slot();
  if (!slot) return 0;

  slot->next = 0;

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
  Slot* slot = objectFindSlot(obj, key);
  if (slot) return &slot->value;

  return objectAdd(obj, key, pool);
}

template <typename TKey>
inline JsonVariantData* objectGet(const JsonObjectData* obj, TKey key) {
  Slot* slot = objectFindSlot(obj, key);
  return slot ? &slot->value : 0;
}

inline void objectClear(JsonObjectData* obj) {
  if (!obj) return;
  obj->head = 0;
  obj->tail = 0;
}

inline void objectRemove(JsonObjectData* obj, Slot* slot) {
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
  for (Slot* s = src->head; s; s = s->next) {
    JsonVariantData* var;
    if (s->value.keyIsStatic)
      var = objectAdd(dst, ZeroTerminatedRamStringConst(s->key), pool);
    else
      var = objectAdd(dst, ZeroTerminatedRamString(s->key), pool);
    if (!variantCopy(var, &s->value, pool)) return false;
  }
  return true;
}

inline bool objectEquals(const JsonObjectData* o1, const JsonObjectData* o2) {
  if (o1 == o2) return true;
  if (!o1 || !o2) return false;

  for (Slot* s = o1->head; s; s = s->next) {
    JsonVariantData* v1 = &s->value;
    JsonVariantData* v2 = objectGet(o2, makeString(s->key));
    if (!variantEquals(v1, v2)) return false;
  }
  return true;
}
}  // namespace ARDUINOJSON_NAMESPACE
