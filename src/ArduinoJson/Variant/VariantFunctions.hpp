// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>
#include <ArduinoJson/Variant/Visitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

bool collectionCopy(CollectionData* dst, const CollectionData* src,
                    MemoryPool* pool);
void collectionRemoveElement(CollectionData* data, size_t index,
                             MemoryPool* pool);

template <typename TVisitor>
inline typename TVisitor::result_type variantAccept(const VariantData* var,
                                                    TVisitor& visitor) {
  if (var != 0)
    return var->accept(visitor);
  else
    return visitor.visitNull();
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
      auto dup = pool->saveString(str);
      if (!dup)
        return false;
      dst->setOwnedString(dup);
      return true;
    }
    case VALUE_IS_RAW_STRING: {
      auto str = adaptString(src->asRawString());
      auto dup = pool->saveString(str);
      if (!dup)
        return false;
      dst->setRawString(dup);
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
  var->setNull(pool);
}

inline void variantSetBoolean(VariantData* var, bool value, MemoryPool* pool) {
  if (!var)
    return;
  var->setBoolean(value, pool);
}

inline void variantSetFloat(VariantData* var, JsonFloat value,
                            MemoryPool* pool) {
  if (!var)
    return;
  var->setFloat(value, pool);
}

template <typename T>
inline void variantSetInteger(VariantData* var, T value, MemoryPool* pool) {
  if (!var)
    return;
  var->setInteger(value, pool);
}

template <typename TAdaptedString>
inline void variantSetString(VariantData* var, TAdaptedString value,
                             MemoryPool* pool) {
  if (!var)
    return;
  var->setString(value, pool);
}

template <typename T>
inline void variantSetRawString(VariantData* var, SerializedValue<T> value,
                                MemoryPool* pool) {
  if (!var)
    return;
  var->setRawString(value, pool);
}

inline size_t variantSize(const VariantData* var) {
  return var != 0 ? var->size() : 0;
}

inline CollectionData* variantToArray(VariantData* var, MemoryPool* pool) {
  if (!var)
    return 0;
  return &var->toArray(pool);
}

inline CollectionData* variantToObject(VariantData* var, MemoryPool* pool) {
  if (!var)
    return 0;
  return &var->toObject(pool);
}

inline VariantData* variantGetElement(const VariantData* var, size_t index) {
  return var != 0 ? var->getElement(index) : 0;
}

inline VariantData* variantAddElement(VariantData* var, MemoryPool* pool) {
  if (!var)
    return nullptr;
  return var->addElement(pool);
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
    slot = new (pool) VariantSlot();
    if (!slot)
      return nullptr;
    array->add(slot);
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
  if (!var)
    return nullptr;
  return var->getOrAddMember(key, pool);
}

template <typename TAdaptedString>
void variantRemoveMember(VariantData* var, TAdaptedString key,
                         MemoryPool* pool) {
  if (!var)
    return;
  var->removeMember(key, pool);
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
