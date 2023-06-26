// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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

  VariantData* getOrAddElement(size_t index, ResourceManager* resources);

  VariantData* getElement(size_t index) const;

  static VariantData* getElement(const ArrayData* array, size_t index) {
    if (!array)
      return nullptr;
    return array->getElement(index);
  }

  void removeElement(size_t index, ResourceManager* resources);

  static void removeElement(ArrayData* array, size_t index,
                            ResourceManager* resources) {
    if (!array)
      return;
    array->removeElement(index, resources);
  }

  bool equals(const ArrayData&) const;

  static bool equals(const ArrayData* lhs, const ArrayData* rhs) {
    if (lhs == rhs)
      return true;
    if (!lhs || !rhs)
      return false;

    return lhs->equals(*rhs);
  }

  bool copyFrom(const ArrayData& src, ResourceManager* resources);

  static bool copy(ArrayData* dst, const ArrayData* src,
                   ResourceManager* resources) {
    if (!dst || !src)
      return false;

    return dst->copyFrom(*src, resources);
  }

 private:
  VariantSlot* getSlot(size_t index) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
