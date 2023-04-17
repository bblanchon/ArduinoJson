// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>
#include <ArduinoJson/Variant/Visitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

void slotRelease(const VariantSlot* slot, MemoryPool* pool);
bool collectionCopy(CollectionData* dst, const CollectionData* src,
                    MemoryPool* pool);
VariantData* collectionAddElement(CollectionData* array, MemoryPool* pool);
void collectionRemoveElement(CollectionData* data, size_t index,
                             MemoryPool* pool);
template <typename TAdaptedString>
void collectionRemoveMember(CollectionData* data, TAdaptedString key,
                            MemoryPool* pool);

template <typename TVisitor>
inline typename TVisitor::result_type variantAccept(const VariantData* var,
                                                    TVisitor& visitor) {
  if (var != 0)
    return var->accept(visitor);
  else
    return visitor.visitNull();
}

inline void variantRelease(const VariantData* var, MemoryPool* pool) {
  ARDUINOJSON_ASSERT(var != nullptr);
  auto c = var->asCollection();
  if (c) {
    for (auto slot = c->head(); slot; slot = slot->next())
      slotRelease(slot, pool);
  }
}

inline bool variantCopyFrom(VariantData* dst, const VariantData* src,
                            MemoryPool* pool) {
  if (!dst)
    return false;
  if (!src) {
    dst->setNull();
    return true;
  }
  switch (src->type()) {
    case VALUE_IS_ARRAY:
      return collectionCopy(&dst->toArray(), src->asArray(), pool);
    case VALUE_IS_OBJECT:
      return collectionCopy(&dst->toObject(), src->asObject(), pool);
    case VALUE_IS_OWNED_STRING: {
      auto str = adaptString(src->asString());
      auto dup = storeString(pool, str, StringStoragePolicy::Copy());
      if (!dup)
        return false;
      dst->setString(dup);
      return true;
    }
    case VALUE_IS_OWNED_RAW: {
      auto str = adaptString(src->asRaw());
      auto dup = storeString(pool, str, StringStoragePolicy::Copy());
      if (!dup)
        return false;
      dst->setOwnedRaw(dup.c_str(), str.size());
      return true;
    }
    default:
      *dst = *src;
      return true;
  }
}

inline void variantSetNull(VariantData* var, MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  var->setNull();
}

inline void variantSetBoolean(VariantData* var, bool value, MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  var->setBoolean(value);
}

inline void variantSetFloat(VariantData* var, JsonFloat value,
                            MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  var->setFloat(value);
}

template <typename T>
inline void variantSetInteger(VariantData* var, T value, MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  var->setInteger(value);
}

template <typename TAdaptedString>
inline void variantSetString(VariantData* var, TAdaptedString value,
                             MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  JsonString str = storeString(pool, value);
  if (str)
    var->setString(str);
  else
    var->setNull();
}

template <typename T>
inline void variantSetOwnedRaw(VariantData* var, SerializedValue<T> value,
                               MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  const char* dup = pool->saveString(adaptString(value.data(), value.size()));
  if (dup)
    var->setOwnedRaw(dup, value.size());
  else
    var->setNull();
}

inline void variantSetLinkedRaw(VariantData* var,
                                SerializedValue<const char*> value,
                                MemoryPool* pool) {
  if (!var)
    return;
  variantRelease(var, pool);
  if (value.data())
    var->setLinkedRaw(value.data(), value.size());
  else
    var->setNull();
}

inline size_t variantSize(const VariantData* var) {
  return var != 0 ? var->size() : 0;
}

inline CollectionData* variantToArray(VariantData* var, MemoryPool* pool) {
  if (!var)
    return 0;
  variantRelease(var, pool);
  return &var->toArray();
}

inline CollectionData* variantToObject(VariantData* var, MemoryPool* pool) {
  if (!var)
    return 0;
  variantRelease(var, pool);
  return &var->toObject();
}

inline VariantData* variantGetElement(const VariantData* var, size_t index) {
  return var != 0 ? var->getElement(index) : 0;
}

inline VariantData* variantAddElement(VariantData* var, MemoryPool* pool) {
  if (!var)
    return nullptr;
  auto array = var->isNull() ? &var->toArray() : var->asArray();
  return collectionAddElement(array, pool);
}

inline NO_INLINE VariantData* variantGetOrAddElement(VariantData* var,
                                                     size_t index,
                                                     MemoryPool* pool) {
  if (!var)
    return nullptr;
  auto array = var->isNull() ? &var->toArray() : var->asArray();
  if (!array)
    return nullptr;
  VariantSlot* slot = array->head();
  while (slot && index > 0) {
    slot = slot->next();
    index--;
  }
  if (!slot)
    index++;
  while (index > 0) {
    slot = pool->allocVariant();
    if (!slot)
      return nullptr;
    array->addSlot(slot);
    index--;
  }
  return slot->data();
}

inline void variantRemoveElement(VariantData* var, size_t index,
                                 MemoryPool* pool) {
  if (!var)
    return;
  collectionRemoveElement(var->asArray(), index, pool);
}

template <typename TAdaptedString>
VariantData* variantGetMember(const VariantData* var, TAdaptedString key) {
  if (!var)
    return 0;
  return var->getMember(key);
}

template <typename TAdaptedString>
VariantData* variantGetOrAddMember(VariantData* var, TAdaptedString key,
                                   MemoryPool* pool) {
  if (!var || key.isNull())
    return nullptr;
  auto obj = var->isNull() ? &var->toObject() : var->asObject();
  if (!obj)
    return nullptr;
  auto slot = obj->getSlot(key);
  if (slot)
    return slot->data();
  return collectionAddMember(obj, key, pool);
}

template <typename TAdaptedString>
void variantRemoveMember(VariantData* var, TAdaptedString key,
                         MemoryPool* pool) {
  if (!var)
    return;
  collectionRemoveMember(var->asObject(), key, pool);
}

inline bool variantIsNull(const VariantData* var) {
  return var == 0 || var->isNull();
}

inline size_t variantNesting(const VariantData* var) {
  if (!var)
    return 0;

  const CollectionData* collection = var->asCollection();
  if (!collection)
    return 0;

  size_t maxChildNesting = 0;
  for (const VariantSlot* s = collection->head(); s; s = s->next()) {
    size_t childNesting = variantNesting(s->data());
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
