// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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

inline VariantData* CollectionData::addElement(ResourceManager* resources) {
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;
  addSlot(slot);
  return slot->data();
}

inline VariantData* CollectionData::addMember(StringNode* key,
                                              ResourceManager* resources) {
  ARDUINOJSON_ASSERT(key != nullptr);
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;

  slot->setKey(key);
  addSlot(slot);
  return slot->data();
}

template <typename TAdaptedString>
inline VariantData* CollectionData::addMember(TAdaptedString key,
                                              ResourceManager* resources) {
  ARDUINOJSON_ASSERT(!key.isNull());
  auto slot = resources->allocVariant();
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
  addSlot(slot);
  return slot->data();
}

template <typename TAdaptedString>
inline VariantData* CollectionData::getMember(TAdaptedString key) const {
  return slotData(getSlot(key));
}

inline VariantData* CollectionData::getOrAddElement(
    size_t index, ResourceManager* resources) {
  VariantSlot* slot = head_;
  while (slot && index > 0) {
    slot = slot->next();
    index--;
  }
  if (!slot)
    index++;
  while (index > 0) {
    slot = resources->allocVariant();
    if (!slot)
      return nullptr;
    addSlot(slot);
    index--;
  }
  return slot->data();
}

template <typename TAdaptedString>
VariantData* CollectionData::getOrAddMember(TAdaptedString key,
                                            ResourceManager* resources) {
  auto slot = getSlot(key);
  if (slot)
    return slot->data();
  return addMember(key, resources);
}

inline VariantData* CollectionData::getElement(size_t index) const {
  return slotData(getSlot(index));
}

inline void CollectionData::clear(ResourceManager* resources) {
  for (auto slot = head_; slot; slot = slot->next())
    slotRelease(slot, resources);
  head_ = 0;
  tail_ = 0;
}

inline bool CollectionData::copyFrom(const CollectionData& src,
                                     ResourceManager* resources) {
  clear(resources);

  for (VariantSlot* s = src.head(); s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      JsonString key(s->key(),
                     s->ownsKey() ? JsonString::Copied : JsonString::Linked);
      var = addMember(adaptString(key), resources);
    } else {
      var = addElement(resources);
    }
    if (!variantCopyFrom(var, s->data(), resources))
      return false;
  }
  return true;
}

template <typename TAdaptedString>
inline VariantSlot* CollectionData::getSlot(TAdaptedString key) const {
  if (key.isNull())
    return 0;
  VariantSlot* slot = head_;
  while (slot) {
    if (stringEquals(key, adaptString(slot->key())))
      break;
    slot = slot->next();
  }
  return slot;
}

inline VariantSlot* CollectionData::getSlot(size_t index) const {
  if (!head_)
    return 0;
  return head_->next(index);
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

inline void CollectionData::removeSlot(VariantSlot* slot,
                                       ResourceManager* resources) {
  if (!slot)
    return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    head_ = next;
  if (!next)
    tail_ = prev;
  slotRelease(slot, resources);
}

inline void CollectionData::removeElement(size_t index,
                                          ResourceManager* resources) {
  removeSlot(getSlot(index), resources);
}

template <typename TAdaptedString>
inline void CollectionData::removeMember(TAdaptedString key,
                                         ResourceManager* resources) {
  removeSlot(getSlot(key), resources);
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

inline size_t CollectionData::size() const {
  return slotSize(head_);
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

inline bool arrayEquals(const CollectionData& lhs, const CollectionData& rhs) {
  auto a = lhs.head();
  auto b = rhs.head();

  for (;;) {
    if (!a && !b)  // both ended
      return true;
    if (!a || !b)  // one ended
      return false;
    if (compare(a->data(), b->data()) != COMPARE_RESULT_EQUAL)
      return false;
    a = a->next();
    b = b->next();
  }
}

inline bool arrayEquals(const CollectionData* lhs, const CollectionData* rhs) {
  if (lhs == rhs)
    return true;
  if (!lhs || !rhs)
    return false;

  return arrayEquals(*lhs, *rhs);
}

inline bool objectEquals(const CollectionData& lhs, const CollectionData& rhs) {
  size_t count = 0;
  for (auto a = lhs.head(); a; a = a->next()) {
    auto b = rhs.getMember(adaptString(a->key()));
    if (!b)
      return false;
    if (compare(a->data(), b) != COMPARE_RESULT_EQUAL)
      return false;
    count++;
  }
  return count == rhs.size();
}

inline bool objectEquals(const CollectionData* lhs, const CollectionData* rhs) {
  if (lhs == rhs)
    return true;

  if (!lhs || !rhs)
    return false;

  return objectEquals(*lhs, *rhs);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
