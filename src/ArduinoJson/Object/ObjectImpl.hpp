// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/ObjectData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline bool ObjectData::copyFrom(const ObjectData& src,
                                 ResourceManager* resources) {
  clear(resources);

  for (auto it = src.createIterator(); !it.done(); it.next()) {
    ARDUINOJSON_ASSERT(it.key() != 0);
    JsonString key(it.key(),
                   it.ownsKey() ? JsonString::Copied : JsonString::Linked);
    auto var = addMember(adaptString(key), resources);
    if (!var)
      return false;
    if (!var->copyFrom(*it, resources))
      return false;
  }
  return true;
}

template <typename TAdaptedString>
inline VariantData* ObjectData::getMember(TAdaptedString key) const {
  return findKey(key).data();
}

template <typename TAdaptedString>
VariantData* ObjectData::getOrAddMember(TAdaptedString key,
                                        ResourceManager* resources) {
  auto it = findKey(key);
  if (!it.done())
    return it.data();
  return addMember(key, resources);
}

template <typename TAdaptedString>
inline ObjectData::iterator ObjectData::findKey(TAdaptedString key) const {
  if (key.isNull())
    return iterator();
  for (auto it = createIterator(); !it.done(); it.next()) {
    if (stringEquals(key, adaptString(it.key())))
      return it;
  }
  return iterator();
}

template <typename TAdaptedString>
inline void ObjectData::removeMember(TAdaptedString key,
                                     ResourceManager* resources) {
  remove(findKey(key), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
