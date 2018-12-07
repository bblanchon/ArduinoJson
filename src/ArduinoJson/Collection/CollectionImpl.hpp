// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Variant/VariantData.hpp"
#include "CollectionData.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline VariantSlot* CollectionData::addSlot(MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot) return 0;

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

inline VariantData* CollectionData::add(MemoryPool* pool) {
  return addSlot(pool)->data();
}

template <typename TKey>
inline VariantData* CollectionData::add(TKey key, MemoryPool* pool) {
  VariantSlot* slot = addSlot(pool);
  if (!slotSetKey(slot, key, pool)) return 0;
  return slot->data();
}

inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}

template <typename TKey>
inline bool CollectionData::containsKey(const TKey& key) const {
  return getSlot(key) != 0;
}

inline bool CollectionData::copyFrom(const CollectionData& src,
                                     MemoryPool* pool) {
  clear();
  for (VariantSlot* s = src._head; s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      if (s->ownsKey())
        var = add(RamStringWrapper(s->key()), pool);
      else
        var = add(ConstRamStringWrapper(s->key()), pool);
    } else {
      var = add(pool);
    }
    if (!var) return false;
    if (!var->copyFrom(*s->data(), pool)) return false;
  }
  return true;
}

inline bool CollectionData::equalsObject(const CollectionData& other) const {
  size_t count = 0;
  for (VariantSlot* slot = _head; slot; slot = slot->next()) {
    VariantData* v1 = slot->data();
    VariantData* v2 = other.get(wrapString(slot->key()));
    if (!variantEquals(v1, v2)) return false;
    count++;
  }
  return count == other.size();
}

inline bool CollectionData::equalsArray(const CollectionData& other) const {
  VariantSlot* s1 = _head;
  VariantSlot* s2 = other._head;
  for (;;) {
    if (s1 == s2) return true;
    if (!s1 || !s2) return false;
    if (!variantEquals(s1->data(), s2->data())) return false;
    s1 = s1->next();
    s2 = s2->next();
  }
}

template <typename TKey>
inline VariantSlot* CollectionData::getSlot(TKey key) const {
  VariantSlot* slot = _head;
  while (slot) {
    if (key.equals(slot->key())) break;
    slot = slot->next();
  }
  return slot;
}

inline VariantSlot* CollectionData::getSlot(size_t index) const {
  return _head->next(index);
}

inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = _head;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target) return current;
    current = next;
  }
  return 0;
}

template <typename TKey>
inline VariantData* CollectionData::get(TKey key) const {
  VariantSlot* slot = getSlot(key);
  return slot ? slot->data() : 0;
}

inline VariantData* CollectionData::get(size_t index) const {
  VariantSlot* slot = getSlot(index);
  return slot ? slot->data() : 0;
}

inline void CollectionData::remove(VariantSlot* slot) {
  if (!slot) return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = next;
  if (!next) _tail = prev;
}

inline void CollectionData::remove(size_t index) {
  remove(getSlot(index));
}

inline size_t CollectionData::size() const {
  return slotSize(_head);
}

}  // namespace ARDUINOJSON_NAMESPACE
