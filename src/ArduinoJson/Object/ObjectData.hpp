// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class ObjectData : public CollectionData {
 public:
  VariantData* addMember(StringNode* key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* addMember(TAdaptedString key, ResourceManager* resources);

  bool copyFrom(const ObjectData& src, ResourceManager* resources);

  static bool copy(ObjectData* dst, const ObjectData* src,
                   ResourceManager* resources) {
    if (!dst || !src)
      return false;

    return dst->copyFrom(*src, resources);
  }

  bool equals(const ObjectData& other) const;

  static bool equals(const ObjectData* lhs, const ObjectData* rhs) {
    if (lhs == rhs)
      return true;

    if (!lhs || !rhs)
      return false;

    return lhs->equals(*rhs);
  }

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key) const;

  template <typename TAdaptedString>
  static VariantData* getMember(const ObjectData* object, TAdaptedString key) {
    if (!object)
      return nullptr;
    return object->getMember(key);
  }

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key, ResourceManager* resources);

  template <typename TAdaptedString>
  static void removeMember(ObjectData* obj, TAdaptedString key,
                           ResourceManager* resources) {
    if (!obj)
      return;
    obj->removeMember(key, resources);
  }

 private:
  template <typename TAdaptedString>
  VariantSlot* getSlot(TAdaptedString key) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
