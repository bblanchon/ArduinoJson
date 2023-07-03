// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline const char* CollectionIterator::key() const {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  return slot_->key();
}

inline bool CollectionIterator::ownsKey() const {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  return slot_->ownsKey();
}

inline void CollectionIterator::next() {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  slot_ = slot_->next();
}

inline void CollectionData::addSlot(VariantSlot* slot) {
  ARDUINOJSON_ASSERT(slot != nullptr);

  if (tail_) {
    tail_->setNextNotNull(slot);
    tail_ = slot;
  } else {
    head_ = slot;
    tail_ = slot;
  }
}

inline void CollectionData::clear(ResourceManager* resources) {
  for (auto slot = head_; slot; slot = slot->next())
    releaseSlot(slot, resources);
  head_ = 0;
  tail_ = 0;
}

inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = head_;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target)
      return current;
    current = next;
  }
  return 0;
}

inline void CollectionData::remove(iterator it, ResourceManager* resources) {
  if (it.done())
    return;
  auto curr = it.slot_;
  auto prev = getPreviousSlot(curr);
  auto next = curr->next();
  if (prev)
    prev->setNext(next);
  else
    head_ = next;
  if (!next)
    tail_ = prev;
  releaseSlot(curr, resources);
}

inline size_t CollectionData::memoryUsage() const {
  size_t total = 0;
  for (VariantSlot* s = head_; s; s = s->next()) {
    total += sizeof(VariantSlot) + s->data()->memoryUsage();
    if (s->ownsKey())
      total += sizeofString(strlen(s->key()));
  }
  return total;
}

inline size_t CollectionData::nesting() const {
  size_t maxChildNesting = 0;
  for (auto it = createIterator(); !it.done(); it.next()) {
    size_t childNesting = it->nesting();
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}

inline size_t CollectionData::size() const {
  size_t count = 0;
  for (auto it = createIterator(); !it.done(); it.next())
    count++;
  return count;
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
  movePointer(head_, variantDistance);
  movePointer(tail_, variantDistance);
  for (VariantSlot* slot = head_; slot; slot = slot->next())
    slot->data()->movePointers(variantDistance);
}

inline void CollectionData::releaseSlot(VariantSlot* slot,
                                        ResourceManager* resources) {
  ARDUINOJSON_ASSERT(slot != nullptr);
  if (slot->ownsKey())
    resources->dereferenceString(slot->key());
  slot->data()->setNull(resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
