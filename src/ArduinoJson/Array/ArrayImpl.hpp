// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline ArrayData::iterator ArrayData::at(size_t index) const {
  auto it = createIterator();
  while (!it.done() && index) {
    it.next();
    --index;
  }
  return it;
}

inline VariantData* ArrayData::getOrAddElement(size_t index,
                                               ResourceManager* resources) {
  auto it = createIterator();
  while (!it.done() && index > 0) {
    it.next();
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

inline VariantData* ArrayData::getElement(size_t index) const {
  return at(index).data();
}

inline void ArrayData::removeElement(size_t index, ResourceManager* resources) {
  remove(at(index), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
