// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/VariantCompare.hpp>
#include <ArduinoJson/Variant/VariantImpl.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TAdaptedString>
inline VariantData* VariantImpl::getMember(TAdaptedString key,
                                           VariantData* data,
                                           ResourceManager* resources) {
  auto it = findKey(key, data, resources);
  if (it.done())
    return nullptr;
  it.move(resources);
  return it.data();
}

template <typename TAdaptedString>
VariantData* VariantImpl::getOrAddMember(TAdaptedString key, VariantData* data,
                                         ResourceManager* resources) {
  ARDUINOJSON_ASSERT(data != nullptr);
  ARDUINOJSON_ASSERT(data->isObject());
  ARDUINOJSON_ASSERT(resources != nullptr);

  auto member = getMember(key, data, resources);
  if (member)
    return member;
  return addMember(key, data, resources);
}

template <typename TAdaptedString>
inline VariantImpl::iterator VariantImpl::findKey(TAdaptedString key,
                                                  VariantData* data,
                                                  ResourceManager* resources) {
  ARDUINOJSON_ASSERT(data != nullptr);
  ARDUINOJSON_ASSERT(data->isObject());
  ARDUINOJSON_ASSERT(resources != nullptr);

  if (key.isNull())
    return iterator();
  bool isKey = true;
  for (auto it = createIterator(data, resources); !it.done();
       it.move(resources)) {
    if (isKey && stringEquals(key, adaptString(it->asString())))
      return it;
    isKey = !isKey;
  }
  return iterator();
}

template <typename TAdaptedString>
inline VariantData* VariantImpl::addMember(TAdaptedString key,
                                           VariantData* data,
                                           ResourceManager* resources) {
  ARDUINOJSON_ASSERT(data != nullptr);
  ARDUINOJSON_ASSERT(data->isObject());
  ARDUINOJSON_ASSERT(resources != nullptr);

  if (key.isNull())
    return nullptr;  // Ignore null key

  auto keySlot = resources->allocVariant();
  if (!keySlot)
    return nullptr;

  auto valueSlot = resources->allocVariant();
  if (!valueSlot)
    return nullptr;

  if (!VariantImpl::setString(key, keySlot.ptr(), resources))
    return nullptr;

  appendPair(keySlot, valueSlot, data, resources);

  return valueSlot.ptr();
}

inline VariantData* VariantImpl::addPair(VariantData** value, VariantData* data,
                                         ResourceManager* resources) {
  ARDUINOJSON_ASSERT(value != nullptr);
  ARDUINOJSON_ASSERT(data != nullptr);
  ARDUINOJSON_ASSERT(data->isObject());
  ARDUINOJSON_ASSERT(resources != nullptr);

  auto keySlot = resources->allocVariant();
  if (!keySlot)
    return nullptr;

  auto valueSlot = resources->allocVariant();
  if (!valueSlot)
    return nullptr;
  *value = valueSlot.ptr();

  appendPair(keySlot, valueSlot, data, resources);

  return keySlot.ptr();
}

// Returns the size (in bytes) of an object with n members.
constexpr size_t sizeofObject(size_t n) {
  return 2 * n * sizeof(VariantData);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
