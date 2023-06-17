// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline VariantData* collectionAddElement(CollectionData* array,
                                         ResourceManager* resources) {
  if (!array)
    return nullptr;
  auto slot = new (resources) VariantSlot();
  if (!slot)
    return nullptr;
  array->add(slot);
  return slot->data();
}

template <typename TAdaptedString>
inline VariantData* collectionAddMember(CollectionData* obj, TAdaptedString key,
                                        ResourceManager* resources) {
  ARDUINOJSON_ASSERT(!key.isNull());
  ARDUINOJSON_ASSERT(obj != nullptr);
  auto slot = new (resources) VariantSlot();
  if (!slot)
    return nullptr;
  if (key.isLinked())
    slot->setKey(key.data());
  else {
    auto storedKey = resources->saveString(key);
    if (!storedKey)
      return nullptr;
    slot->setKey(storedKey);
  }
  obj->add(slot);
  return slot->data();
}

inline void collectionClear(CollectionData* c, ResourceManager* resources) {
  if (!c)
    return;
  for (auto slot = c->head(); slot; slot = slot->next())
    slotRelease(slot, resources);
  c->clear();
}

inline bool collectionCopy(CollectionData* dst, const CollectionData* src,
                           ResourceManager* resources) {
  if (!dst || !src)
    return false;

  collectionClear(dst, resources);

  for (VariantSlot* s = src->head(); s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      JsonString key(s->key(),
                     s->ownsKey() ? JsonString::Copied : JsonString::Linked);
      var = collectionAddMember(dst, adaptString(key), resources);
    } else {
      var = collectionAddElement(dst, resources);
    }
    if (!variantCopyFrom(var, s->data(), resources))
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
                             ResourceManager* resources) {
  if (!data || !slot)
    return;
  data->remove(slot);
  slotRelease(slot, resources);
}

inline void collectionRemoveElement(CollectionData* array, size_t index,
                                    ResourceManager* resources) {
  if (!array)
    return;
  collectionRemove(array, array->get(index), resources);
}

template <typename TAdaptedString>
inline void collectionRemoveMember(CollectionData* obj, TAdaptedString key,
                                   ResourceManager* resources) {
  if (!obj)
    return;
  collectionRemove(obj, obj->get(key), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
