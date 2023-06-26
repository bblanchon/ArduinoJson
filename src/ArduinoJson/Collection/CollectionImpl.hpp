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

template <typename TAdaptedString>
VariantData* CollectionData::getOrAddMember(TAdaptedString key,
                                            ResourceManager* resources) {
  auto slot = getSlot(key);
  if (slot)
    return slot->data();
  return addMember(key, resources);
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
    ARDUINOJSON_ASSERT(s->key() != 0);
    JsonString key(s->key(),
                   s->ownsKey() ? JsonString::Copied : JsonString::Linked);
    auto var = addMember(adaptString(key), resources);
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
