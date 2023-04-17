// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline void CollectionData::add(VariantSlot* slot) {
  ARDUINOJSON_ASSERT(slot != nullptr);

  if (_tail) {
    _tail->setNextNotNull(slot);
    _tail = slot;
  } else {
    _head = slot;
    _tail = slot;
  }
}

inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}

template <typename TAdaptedString>
inline VariantSlot* CollectionData::get(TAdaptedString key) const {
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

inline VariantSlot* CollectionData::get(size_t index) const {
  if (!_head)
    return 0;
  return _head->next(index);
}

inline VariantSlot* CollectionData::getPrevious(VariantSlot* target) const {
  VariantSlot* current = _head;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target)
      return current;
    current = next;
  }
  return 0;
}

inline void CollectionData::remove(VariantSlot* slot) {
  if (!slot)
    return;
  VariantSlot* prev = getPrevious(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = next;
  if (!next)
    _tail = prev;
}

inline size_t CollectionData::memoryUsage() const {
  size_t total = 0;
  for (VariantSlot* s = _head; s; s = s->next()) {
    total += sizeof(VariantSlot) + s->data()->memoryUsage();
    if (s->ownsKey())
      total += sizeofString(strlen(s->key()));
  }
  return total;
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

inline void CollectionData::movePointers(ptrdiff_t variantDistance) {
  movePointer(_head, variantDistance);
  movePointer(_tail, variantDistance);
  for (VariantSlot* slot = _head; slot; slot = slot->next())
    slot->movePointers(variantDistance);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
