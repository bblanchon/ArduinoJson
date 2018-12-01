// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Variant/SlotFunctions.hpp"
#include "../Variant/VariantData.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TKey>
inline VariantSlot* objectFindSlot(const ObjectData* obj, TKey key) {
  if (!obj) return 0;
  VariantSlot* slot = obj->head;
  while (slot) {
    if (key.equals(slotGetKey(slot))) break;
    slot = slot->getNext();
  }
  return slot;
}

template <typename TKey>
inline bool objectContainsKey(const ObjectData* obj, const TKey& key) {
  return objectFindSlot(obj, key) != 0;
}

template <typename TKey>
inline VariantData* objectAdd(ObjectData* obj, TKey key, MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot) return 0;

  slot->init();

  if (obj->tail) {
    slot->attachTo(obj->tail);
    obj->tail = slot;
  } else {
    obj->head = slot;
    obj->tail = slot;
  }

  if (!slotSetKey(slot, key, pool)) return 0;
  return slot->getData();
}

template <typename TKey>
inline VariantData* objectSet(ObjectData* obj, TKey key, MemoryPool* pool) {
  if (!obj) return 0;

  // ignore null key
  if (key.isNull()) return 0;

  // search a matching key
  VariantSlot* slot = objectFindSlot(obj, key);
  if (slot) return slot->getData();

  return objectAdd(obj, key, pool);
}

template <typename TKey>
inline VariantData* objectGet(const ObjectData* obj, TKey key) {
  VariantSlot* slot = objectFindSlot(obj, key);
  return slot ? slot->getData() : 0;
}

inline void objectClear(ObjectData* obj) {
  if (!obj) return;
  obj->head = 0;
  obj->tail = 0;
}

inline void objectRemove(ObjectData* obj, VariantSlot* slot) {
  if (!obj) return;
  if (!slot) return;
  VariantSlot* prev = slot->getPrev(obj->head);
  VariantSlot* next = slot->getNext();
  if (prev)
    prev->setNext(next);
  else
    obj->head = next;
  if (!next) obj->tail = prev;
}

inline size_t objectSize(const ObjectData* obj) {
  if (!obj) return 0;
  return slotSize(obj->head);
}

// bool variantCopy(VariantData*, const VariantData*, MemoryPool*);

inline bool objectCopy(ObjectData* dst, const ObjectData* src,
                       MemoryPool* pool) {
  if (!dst || !src) return false;
  objectClear(dst);
  for (VariantSlot* s = src->head; s; s = s->getNext()) {
    VariantData* var;
    if (s->ownsKey())
      var = objectAdd(dst, ZeroTerminatedRamString(s->key()), pool);
    else
      var = objectAdd(dst, ZeroTerminatedRamStringConst(s->key()), pool);
    if (!variantCopy(var, s->getData(), pool)) return false;
  }
  return true;
}

inline bool objectEquals(const ObjectData* o1, const ObjectData* o2) {
  if (o1 == o2) return true;
  if (!o1 || !o2) return false;

  for (VariantSlot* s = o1->head; s; s = s->getNext()) {
    VariantData* v1 = s->getData();
    VariantData* v2 = objectGet(o2, makeString(slotGetKey(s)));
    if (!variantEquals(v1, v2)) return false;
  }
  return true;
}
}  // namespace ARDUINOJSON_NAMESPACE
