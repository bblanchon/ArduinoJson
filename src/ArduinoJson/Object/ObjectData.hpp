// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class ObjectData : public CollectionData {
 public:
  template <typename TAdaptedString>  // also works with StringNode*
  VariantData* addMember(TAdaptedString key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* getOrAddMember(TAdaptedString key, ResourceManager* resources);

  template <typename TAdaptedString>
  VariantData* getMember(TAdaptedString key,
                         const ResourceManager* resources) const;

  template <typename TAdaptedString>
  static VariantData* getMember(const ObjectData* object, TAdaptedString key,
                                const ResourceManager* resources) {
    if (!object)
      return nullptr;
    return object->getMember(key, resources);
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

  void remove(iterator it, ResourceManager* resources) {
    CollectionData::removePair(it, resources);
  }

  static void remove(ObjectData* obj, ObjectData::iterator it,
                     ResourceManager* resources) {
    if (!obj)
      return;
    obj->remove(it, resources);
  }

  size_t size(const ResourceManager* resources) const {
    return CollectionData::size(resources) / 2;
  }

  static size_t size(const ObjectData* obj, const ResourceManager* resources) {
    if (!obj)
      return 0;
    return obj->size(resources);
  }

 private:
  template <typename TAdaptedString>
  iterator findKey(TAdaptedString key, const ResourceManager* resources) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
