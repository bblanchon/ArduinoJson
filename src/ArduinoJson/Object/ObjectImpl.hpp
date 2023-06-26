// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/ObjectData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

inline VariantData* ObjectData::addMember(StringNode* key,
                                          ResourceManager* resources) {
  ARDUINOJSON_ASSERT(key != nullptr);
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;

  slot->setKey(key);
  addSlot(slot);
  return slot->data();
}

template <typename TAdaptedString>
inline VariantData* ObjectData::addMember(TAdaptedString key,
                                          ResourceManager* resources) {
  ARDUINOJSON_ASSERT(!key.isNull());
  auto slot = resources->allocVariant();
  if (!slot)
    return nullptr;
  if (key.isLinked())
    slot->setKey(key.data());
  else {
    auto storedKey = resources->saveString(key);
    if (!storedKey)
      return nullptr;
    slot->setKey(storedKey);
  }
  addSlot(slot);
  return slot->data();
}

inline bool ObjectData::copyFrom(const ObjectData& src,
                                 ResourceManager* resources) {
  clear(resources);

  for (auto it = src.begin(); it; ++it) {
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

inline bool ObjectData::equals(const ObjectData& other) const {
  size_t count = 0;
  for (auto it = begin(); it; ++it) {
    auto a = it.data();
    auto b = other.getMember(adaptString(it.key()));
    if (!b)
      return false;
    if (compare(a, b) != COMPARE_RESULT_EQUAL)
      return false;
    count++;
  }
  return count == other.size();
}

template <typename TAdaptedString>
inline VariantData* ObjectData::getMember(TAdaptedString key) const {
  return findKey(key).data();
}

template <typename TAdaptedString>
VariantData* ObjectData::getOrAddMember(TAdaptedString key,
                                        ResourceManager* resources) {
  auto it = findKey(key);
  if (it)
    return it.data();
  return addMember(key, resources);
}

template <typename TAdaptedString>
inline ObjectData::iterator ObjectData::findKey(TAdaptedString key) const {
  if (key.isNull())
    return end();
  for (auto it = begin(); it; ++it) {
    if (stringEquals(key, adaptString(it.key())))
      return it;
  }
  return end();
}

template <typename TAdaptedString>
inline void ObjectData::removeMember(TAdaptedString key,
                                     ResourceManager* resources) {
  remove(findKey(key), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
