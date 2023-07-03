// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Collection/CollectionData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class ObjectData : public CollectionData {
 public:
  VariantData* addMember(StringNode* key, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(key != nullptr);
    auto it = addSlot(resources);
    if (it.done())
      return nullptr;

    it.setKey(key);
    return it.data();
  }

  template <typename TAdaptedString>
  VariantData* addMember(TAdaptedString key, ResourceManager* resources) {
    ARDUINOJSON_ASSERT(!key.isNull());
    if (key.isLinked()) {
      auto it = addSlot(resources);
      if (!it.done())
        it.setKey(key.data());
      return it.data();
    } else {
      auto storedKey = resources->saveString(key);
      if (!storedKey)
        return nullptr;
      auto it = addSlot(resources);
      if (!it.done())
        it.setKey(storedKey);
      return it.data();
    }
  }

  bool copyFrom(const ObjectData& src, ResourceManager* resources);

  static bool copy(ObjectData* dst, const ObjectData* src,
                   ResourceManager* resources) {
    if (!dst || !src)
      return false;

    return dst->copyFrom(*src, resources);
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
  iterator findKey(TAdaptedString key) const;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
