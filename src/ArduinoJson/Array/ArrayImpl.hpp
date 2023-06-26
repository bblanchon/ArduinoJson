// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline ArrayData::iterator ArrayData::at(size_t index) const {
  auto it = begin();
  while (it && index) {
    ++it;
    --index;
  }
  return it;
}

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

  for (auto it = src.begin(); it; ++it) {
    auto var = addElement(resources);
    if (!var)
      return false;
    if (!var->copyFrom(*it, resources))
      return false;
  }
  return true;
}

inline bool ArrayData::equals(const ArrayData& other) const {
  auto a = begin();
  auto b = other.begin();

  for (;;) {
    if (!a && !b)  // both ended
      return true;
    if (!a || !b)  // one ended
      return false;
    if (compare(a.data(), b.data()) != COMPARE_RESULT_EQUAL)
      return false;
    ++a;
    ++b;
  }
}

inline VariantData* ArrayData::getOrAddElement(size_t index,
                                               ResourceManager* resources) {
  auto it = begin();
  while (it && index > 0) {
    ++it;
    index--;
  }
  if (!it)
    index++;
  VariantData* element = it.data();
  while (index > 0) {
    element = addElement(resources);
    if (!element)
      return nullptr;
    index--;
  }
  return element;
}

inline VariantData* ArrayData::getElement(size_t index) const {
  return at(index).data();
}

inline void ArrayData::removeElement(size_t index, ResourceManager* resources) {
  remove(at(index), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
