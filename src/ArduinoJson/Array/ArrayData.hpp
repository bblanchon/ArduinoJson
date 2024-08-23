// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class ArrayData : public CollectionData {
 public:
  VariantData* addElement(ResourceManager* resources);

  static VariantData* addElement(ArrayData* array, ResourceManager* resources) {
    if (!array)
      return nullptr;
    return array->addElement(resources);
  }

  template <typename T>
  bool addValue(T&& value, ResourceManager* resources);

  template <typename T>
  static bool addValue(ArrayData* array, T&& value,
                       ResourceManager* resources) {
    if (!array)
      return false;
    return array->addValue(value, resources);
  }

  VariantData* getOrAddElement(size_t index, ResourceManager* resources);

  VariantData* getElement(size_t index, const ResourceManager* resources) const;

  static VariantData* getElement(const ArrayData* array, size_t index,
                                 const ResourceManager* resources) {
    if (!array)
      return nullptr;
    return array->getElement(index, resources);
  }

  void removeElement(size_t index, ResourceManager* resources);

  static void removeElement(ArrayData* array, size_t index,
                            ResourceManager* resources) {
    if (!array)
      return;
    array->removeElement(index, resources);
  }

  void remove(iterator it, ResourceManager* resources) {
    CollectionData::removeOne(it, resources);
  }

  static void remove(ArrayData* array, iterator it,
                     ResourceManager* resources) {
    if (array)
      return array->remove(it, resources);
  }

 private:
  iterator at(size_t index, const ResourceManager* resources) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
