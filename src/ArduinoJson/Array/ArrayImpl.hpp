// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

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

ARDUINOJSON_END_PRIVATE_NAMESPACE
