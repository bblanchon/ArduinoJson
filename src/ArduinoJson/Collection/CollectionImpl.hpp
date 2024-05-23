// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline CollectionIterator::CollectionIterator(VariantSlot* slot, SlotId slotId)
    : slot_(slot), currentId_(slotId) {
  nextId_ = slot_ ? slot_->next() : NULL_SLOT;
}

inline const char* CollectionIterator::key() const {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  return slot_->key();
}

inline void CollectionIterator::setKey(const char* s) {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  ARDUINOJSON_ASSERT(s != nullptr);
  return slot_->setKey(s);
}

inline void CollectionIterator::setKey(StringNode* s) {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  ARDUINOJSON_ASSERT(s != nullptr);
  return slot_->setKey(s);
}

inline bool CollectionIterator::ownsKey() const {
  ARDUINOJSON_ASSERT(slot_ != nullptr);
  return slot_->ownsKey();
}

inline void CollectionIterator::next(const ResourceManager* resources) {
  ARDUINOJSON_ASSERT(currentId_ != NULL_SLOT);
  slot_ = resources->getSlot(nextId_);
  currentId_ = nextId_;
  if (slot_)
    nextId_ = slot_->next();
}

inline CollectionData::iterator CollectionData::addSlot(
    ResourceManager* resources) {
  auto slot = resources->allocSlot();
  if (!slot)
    return {};
  if (tail_ != NULL_SLOT) {
    auto tail = resources->getSlot(tail_);
    tail->setNext(slot.id());
    tail_ = slot.id();
  } else {
    head_ = slot.id();
    tail_ = slot.id();
  }
  return iterator(slot, slot.id());
}

inline void CollectionData::addSlot(SlotWithId slot,
                                    ResourceManager* resources) {
  if (tail_ != NULL_SLOT) {
    auto tail = resources->getSlot(tail_);
    tail->setNext(slot.id());
    tail_ = slot.id();
  } else {
    head_ = slot.id();
    tail_ = slot.id();
  }
}

inline void CollectionData::clear(ResourceManager* resources) {
  auto next = head_;
  while (next != NULL_SLOT) {
    auto currId = next;
    auto slot = resources->getSlot(next);
    next = slot->next();
    resources->freeSlot(SlotWithId(slot, currId));
  }

  head_ = NULL_SLOT;
  tail_ = NULL_SLOT;
}

inline SlotWithId CollectionData::getPreviousSlot(
    VariantSlot* target, const ResourceManager* resources) const {
  auto prev = SlotWithId();
  auto currentId = head_;
  while (currentId != NULL_SLOT) {
    auto currentSlot = resources->getSlot(currentId);
    if (currentSlot == target)
      return prev;
    prev = SlotWithId(currentSlot, currentId);
    currentId = currentSlot->next();
  }
  return SlotWithId();
}

inline void CollectionData::remove(iterator it, ResourceManager* resources) {
  if (it.done())
    return;
  auto curr = it.slot_;
  auto prev = getPreviousSlot(curr, resources);
  auto next = curr->next();
  if (prev)
    prev->setNext(next);
  else
    head_ = next;
  if (next == NULL_SLOT)
    tail_ = prev.id();
  resources->freeSlot({it.slot_, it.currentId_});
}

inline size_t CollectionData::nesting(const ResourceManager* resources) const {
  size_t maxChildNesting = 0;
  for (auto it = createIterator(resources); !it.done(); it.next(resources)) {
    size_t childNesting = it->nesting(resources);
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}

inline size_t CollectionData::size(const ResourceManager* resources) const {
  size_t count = 0;
  for (auto it = createIterator(resources); !it.done(); it.next(resources))
    count++;
  return count;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
