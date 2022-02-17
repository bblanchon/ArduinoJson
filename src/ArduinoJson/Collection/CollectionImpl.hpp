// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline bool variantEquals(const VariantData* a, const VariantData* b) {
  return variantCompare(a, b) == COMPARE_RESULT_EQUAL;
}

inline VariantSlot* CollectionData::addSlot(MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot)
    return 0;

  if (_tail) {
    _tail->setNextNotNull(slot);
    _tail = slot;
  } else {
    _head = slot;
    _tail = slot;
  }

  slot->clear();
  return slot;
}

inline VariantData* CollectionData::addElement(MemoryPool* pool) {
  return slotData(addSlot(pool));
}

template <typename TAdaptedString, typename TStoragePolicy>
inline VariantData* CollectionData::addMember(TAdaptedString key,
                                              MemoryPool* pool,
                                              TStoragePolicy storage) {
  VariantSlot* slot = addSlot(pool);
  if (!slotSetKey(slot, key, pool, storage)) {
    removeSlot(slot);
    return 0;
  }
  return slot->data();
}

inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}

template <typename TAdaptedString>
inline bool CollectionData::containsKey(const TAdaptedString& key) const {
  return getSlot(key) != 0;
}

inline bool CollectionData::copyFrom(const CollectionData& src,
                                     MemoryPool* pool) {
  clear();
  for (VariantSlot* s = src._head; s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      String key(s->key(), s->ownsKey() ? String::Copied : String::Linked);
      var = addMember(adaptString(key), pool, getStringStoragePolicy(key));
    } else {
      var = addElement(pool);
    }
    if (!var)
      return false;
    if (!var->copyFrom(*s->data(), pool))
      return false;
  }
  return true;
}

inline bool CollectionData::equalsObject(const CollectionData& other) const {
  size_t count = 0;
  for (VariantSlot* slot = _head; slot; slot = slot->next()) {
    VariantData* v1 = slot->data();
    VariantData* v2 = other.getMember(adaptString(slot->key()));
    if (!variantEquals(v1, v2))
      return false;
    count++;
  }
  return count == other.size();
}

inline bool CollectionData::equalsArray(const CollectionData& other) const {
  VariantSlot* s1 = _head;
  VariantSlot* s2 = other._head;
  for (;;) {
    if (s1 == s2)
      return true;
    if (!s1 || !s2)
      return false;
    if (!variantEquals(s1->data(), s2->data()))
      return false;
    s1 = s1->next();
    s2 = s2->next();
  }
}

template <typename TAdaptedString>
inline VariantSlot* CollectionData::getSlot(TAdaptedString key) const {
  if (key.isNull())
    return 0;
  VariantSlot* slot = _head;
  while (slot) {
    if (stringEquals(key, adaptString(slot->key())))
      break;
    slot = slot->next();
  }
  return slot;
}

inline VariantSlot* CollectionData::getSlot(size_t index) const {
  if (!_head)
    return 0;
  return _head->next(index);
}

inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = _head;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target)
      return current;
    current = next;
  }
  return 0;
}

template <typename TAdaptedString>
inline VariantData* CollectionData::getMember(TAdaptedString key) const {
  VariantSlot* slot = getSlot(key);
  return slot ? slot->data() : 0;
}

template <typename TAdaptedString, typename TStoragePolicy>
inline VariantData* CollectionData::getOrAddMember(
    TAdaptedString key, MemoryPool* pool, TStoragePolicy storage_policy) {
  // ignore null key
  if (key.isNull())
    return 0;

  // search a matching key
  VariantSlot* slot = getSlot(key);
  if (slot)
    return slot->data();

  return addMember(key, pool, storage_policy);
}

inline VariantData* CollectionData::getElement(size_t index) const {
  VariantSlot* slot = getSlot(index);
  return slot ? slot->data() : 0;
}

inline VariantData* CollectionData::getOrAddElement(size_t index,
                                                    MemoryPool* pool) {
  VariantSlot* slot = _head;
  while (slot && index > 0) {
    slot = slot->next();
    index--;
  }
  if (!slot)
    index++;
  while (index > 0) {
    slot = addSlot(pool);
    index--;
  }
  return slotData(slot);
}

inline void CollectionData::removeSlot(VariantSlot* slot) {
  if (!slot)
    return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = next;
  if (!next)
    _tail = prev;
}

inline void CollectionData::removeElement(size_t index) {
  removeSlot(getSlot(index));
}

inline size_t CollectionData::memoryUsage() const {
  size_t total = 0;
  for (VariantSlot* s = _head; s; s = s->next()) {
    total += sizeof(VariantSlot) + s->data()->memoryUsage();
    if (s->ownsKey())
      total += strlen(s->key()) + 1;
  }
  return total;
}

inline size_t CollectionData::nesting() const {
  size_t maxChildNesting = 0;
  for (VariantSlot* s = _head; s; s = s->next()) {
    size_t childNesting = s->data()->nesting();
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}

inline size_t CollectionData::size() const {
  return slotSize(_head);
}

template <typename T>
inline void movePointer(T*& p, ptrdiff_t offset) {
  if (!p)
    return;
  p = reinterpret_cast<T*>(
      reinterpret_cast<void*>(reinterpret_cast<char*>(p) + offset));
  ARDUINOJSON_ASSERT(isAligned(p));
}

inline void CollectionData::movePointers(ptrdiff_t stringDistance,
                                         ptrdiff_t variantDistance) {
  movePointer(_head, variantDistance);
  movePointer(_tail, variantDistance);
  for (VariantSlot* slot = _head; slot; slot = slot->next())
    slot->movePointers(stringDistance, variantDistance);
}

}  // namespace ARDUINOJSON_NAMESPACE
