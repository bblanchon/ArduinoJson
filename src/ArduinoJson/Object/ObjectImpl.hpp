// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/ObjectData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TAdaptedString>
inline VariantData* ObjectData::getMember(
    TAdaptedString key, const ResourceManager* resources) const {
  return findKey(key, resources).data();
}

template <typename TAdaptedString>
VariantData* ObjectData::getOrAddMember(TAdaptedString key,
                                        ResourceManager* resources) {
  auto it = findKey(key, resources);
  if (!it.done())
    return it.data();
  return addMember(key, resources);
}

template <typename TAdaptedString>
inline ObjectData::iterator ObjectData::findKey(
    TAdaptedString key, const ResourceManager* resources) const {
  if (key.isNull())
    return iterator();
  for (auto it = createIterator(resources); !it.done(); it.next(resources)) {
    if (stringEquals(key, adaptString(it.key())))
      return it;
  }
  return iterator();
}

template <typename TAdaptedString>
inline void ObjectData::removeMember(TAdaptedString key,
                                     ResourceManager* resources) {
  remove(findKey(key, resources), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
