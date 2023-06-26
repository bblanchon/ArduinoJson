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

  for (VariantSlot* s = src.head(); s; s = s->next()) {
    ARDUINOJSON_ASSERT(s->key() != 0);
    JsonString key(s->key(),
                   s->ownsKey() ? JsonString::Copied : JsonString::Linked);
    auto var = addMember(adaptString(key), resources);
    if (!variantCopyFrom(var, s->data(), resources))
      return false;
  }
  return true;
}

inline bool ObjectData::equals(const ObjectData& other) const {
  size_t count = 0;
  for (auto a = head(); a; a = a->next()) {
    auto b = other.getMember(adaptString(a->key()));
    if (!b)
      return false;
    if (compare(a->data(), b) != COMPARE_RESULT_EQUAL)
      return false;
    count++;
  }
  return count == other.size();
}

template <typename TAdaptedString>
inline VariantData* ObjectData::getMember(TAdaptedString key) const {
  return slotData(getSlot(key));
}

template <typename TAdaptedString>
VariantData* ObjectData::getOrAddMember(TAdaptedString key,
                                        ResourceManager* resources) {
  auto slot = getSlot(key);
  if (slot)
    return slot->data();
  return addMember(key, resources);
}

template <typename TAdaptedString>
inline VariantSlot* ObjectData::getSlot(TAdaptedString key) const {
  if (key.isNull())
    return 0;
  VariantSlot* slot = head();
  while (slot) {
    if (stringEquals(key, adaptString(slot->key())))
      break;
    slot = slot->next();
  }
  return slot;
}

template <typename TAdaptedString>
inline void ObjectData::removeMember(TAdaptedString key,
                                     ResourceManager* resources) {
  removeSlot(getSlot(key), resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
