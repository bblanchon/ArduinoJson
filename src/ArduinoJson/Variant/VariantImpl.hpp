// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
inline void VariantData::setRawString(SerializedValue<T> value,
                                      ResourceManager* resources) {
  ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
  auto dup = resources->saveString(adaptString(value.data(), value.size()));
  if (dup)
    setRawString(dup);
}

template <typename TAdaptedString>
inline bool VariantData::setString(TAdaptedString value,
                                   ResourceManager* resources) {
  ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first

  if (value.isNull())
    return false;

  if (value.isLinked()) {
    setLinkedString(value.data());
    return true;
  }

  auto dup = resources->saveString(value);
  if (dup) {
    setOwnedString(dup);
    return true;
  }

  return false;
}

inline void VariantData::clear(ResourceManager* resources) {
  if (type_ & OWNED_VALUE_BIT)
    resources->dereferenceString(content_.asOwnedString->data);

  auto collection = asCollection();
  if (collection)
    collection->clear(resources);

  type_ = VALUE_IS_NULL;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
