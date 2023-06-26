// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline VariantData* ArrayData::addElement(ResourceManager* resources) {
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;
  addSlot(slot);
  return slot->data();
}

inline bool ArrayData::copyFrom(const ArrayData& src,
                                ResourceManager* resources) {
  clear(resources);

  for (VariantSlot* s = src.head(); s; s = s->next()) {
    auto var = addElement(resources);
    if (!var)
      return false;
    if (!var->copyFrom(s->data(), resources))
      return false;
  }
  return true;
}

inline bool ArrayData::equals(const ArrayData& other) const {
  auto a = head();
  auto b = other.head();

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

inline VariantData* ArrayData::getOrAddElement(size_t index,
                                               ResourceManager* resources) {
  VariantSlot* slot = head();
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

inline VariantData* ArrayData::getElement(size_t index) const {
  return slotData(getSlot(index));
}

inline VariantSlot* ArrayData::getSlot(size_t index) const {
  if (!head())
    return 0;
  return head()->next(index);
}

inline void ArrayData::removeElement(size_t index, ResourceManager* resources) {
  removeSlot(getSlot(index), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
