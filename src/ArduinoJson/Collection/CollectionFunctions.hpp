// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline VariantData* collectionAddElement(CollectionData* array,
                                         MemoryPool* pool) {
  if (!array)
    return nullptr;
  auto slot = pool->allocVariant();
  if (!slot)
    return nullptr;
  array->add(slot);
  return slot->data();
}

template <typename TAdaptedString>
inline VariantData* collectionAddMember(CollectionData* obj, TAdaptedString key,
                                        MemoryPool* pool) {
  ARDUINOJSON_ASSERT(!key.isNull());
  ARDUINOJSON_ASSERT(obj != nullptr);
  auto slot = pool->allocVariant();
  if (!slot)
    return nullptr;
  auto storedKey = storeString(pool, key);
  if (!storedKey)
    return nullptr;
  slot->setKey(storedKey);
  obj->add(slot);
  return slot->data();
}

inline void collectionClear(CollectionData* c, MemoryPool* pool) {
  if (!c)
    return;
  for (auto slot = c->head(); slot; slot = slot->next())
    slotRelease(slot, pool);
  c->clear();
}

inline bool collectionCopy(CollectionData* dst, const CollectionData* src,
                           MemoryPool* pool) {
  if (!dst || !src)
    return false;

  collectionClear(dst, pool);

  for (VariantSlot* s = src->head(); s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      JsonString key(s->key(),
                     s->ownsKey() ? JsonString::Copied : JsonString::Linked);
      var = collectionAddMember(dst, adaptString(key), pool);
    } else {
      var = collectionAddElement(dst, pool);
    }
    if (!variantCopyFrom(var, s->data(), pool))
      return false;
  }
  return true;
}

template <typename TAdaptedString>
inline VariantData* collectionGetMember(const CollectionData* obj,
                                        TAdaptedString key) {
  if (!obj)
    return nullptr;
  return slotData(obj->get(key));
}

inline void collectionRemove(CollectionData* data, VariantSlot* slot,
                             MemoryPool* pool) {
  if (!data || !slot)
    return;
  data->remove(slot);
  slotRelease(slot, pool);
}

inline void collectionRemoveElement(CollectionData* array, size_t index,
                                    MemoryPool* pool) {
  if (!array)
    return;
  collectionRemove(array, array->get(index), pool);
}

template <typename TAdaptedString>
inline void collectionRemoveMember(CollectionData* obj, TAdaptedString key,
                                   MemoryPool* pool) {
  if (!obj)
    return;
  collectionRemove(obj, obj->get(key), pool);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
