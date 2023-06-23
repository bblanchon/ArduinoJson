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
  return array->addElement(resources);
}

template <typename TAdaptedString>
inline VariantData* collectionAddMember(CollectionData* obj, TAdaptedString key,
                                        ResourceManager* resources) {
  ARDUINOJSON_ASSERT(obj != nullptr);
  return obj->addMember(key, resources);
}

inline void collectionClear(CollectionData* c, ResourceManager* resources) {
  if (!c)
    return;
  c->clear(resources);
}

inline bool collectionCopy(CollectionData* dst, const CollectionData* src,
                           ResourceManager* resources) {
  if (!dst || !src)
    return false;

  return dst->copyFrom(*src, resources);
}

inline VariantData* collectionGetElement(const CollectionData* obj,
                                         size_t index) {
  if (!obj)
    return nullptr;
  return obj->getElement(index);
}

template <typename TAdaptedString>
inline VariantData* collectionGetMember(const CollectionData* obj,
                                        TAdaptedString key) {
  if (!obj)
    return nullptr;
  return obj->getMember(key);
}

inline void collectionRemove(CollectionData* data, VariantSlot* slot,
                             ResourceManager* resources) {
  if (!data)
    return;
  data->removeSlot(slot, resources);
}

inline void collectionRemoveElement(CollectionData* array, size_t index,
                                    ResourceManager* resources) {
  if (!array)
    return;
  array->removeElement(index, resources);
}

template <typename TAdaptedString>
inline void collectionRemoveMember(CollectionData* obj, TAdaptedString key,
                                   ResourceManager* resources) {
  if (!obj)
    return;
  obj->removeMember(key, resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
