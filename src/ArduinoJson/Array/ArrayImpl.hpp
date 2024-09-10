// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline ArrayData::iterator ArrayData::at(
    size_t index, const ResourceManager* resources) const {
  auto it = createIterator(resources);
  while (!it.done() && index) {
    it.next(resources);
    --index;
  }
  return it;
}

inline VariantData* ArrayData::addElement(ResourceManager* resources) {
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;
  CollectionData::appendOne(slot, resources);
  return slot.ptr();
}

inline VariantData* ArrayData::getOrAddElement(size_t index,
                                               ResourceManager* resources) {
  auto it = createIterator(resources);
  while (!it.done() && index > 0) {
    it.next(resources);
    index--;
  }
  if (it.done())
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

inline VariantData* ArrayData::getElement(
    size_t index, const ResourceManager* resources) const {
  return at(index, resources).data();
}

inline void ArrayData::removeElement(size_t index, ResourceManager* resources) {
  remove(at(index, resources), resources);
}

template <typename T>
inline bool ArrayData::addValue(T&& value, ResourceManager* resources) {
  ARDUINOJSON_ASSERT(resources != nullptr);
  auto slot = resources->allocVariant();
  if (!slot)
    return false;
  JsonVariant variant(slot.ptr(), resources);
  if (!variant.set(detail::forward<T>(value))) {
    resources->freeVariant(slot);
    return false;
  }
  CollectionData::appendOne(slot, resources);
  return true;
}

// Returns the size (in bytes) of an array with n elements.
constexpr size_t sizeofArray(size_t n) {
  return n * ResourceManager::slotSize;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
