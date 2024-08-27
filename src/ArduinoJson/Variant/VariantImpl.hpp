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

#if ARDUINOJSON_USE_EXTENSIONS
  if (type_ & EXTENSION_BIT)
    resources->freeExtension(content_.asSlotId);
#endif

  auto collection = asCollection();
  if (collection)
    collection->clear(resources);

  type_ = VALUE_IS_NULL;
}

#if ARDUINOJSON_USE_EXTENSIONS
inline const VariantExtension* VariantData::getExtension(
    const ResourceManager* resources) const {
  ARDUINOJSON_ASSERT(type_ & EXTENSION_BIT);
  return resources->getExtension(content_.asSlotId);
}
#endif

template <typename T>
enable_if_t<sizeof(T) == 8, bool> VariantData::setFloat(
    T value, ResourceManager* resources) {
  ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
  (void)resources;                             // silence warning

  float valueAsFloat = static_cast<float>(value);

#if ARDUINOJSON_USE_DOUBLE
  if (value == valueAsFloat) {
    type_ = VALUE_IS_FLOAT;
    content_.asFloat = valueAsFloat;
  } else {
    auto extension = resources->allocExtension();
    if (!extension)
      return false;
    type_ = VALUE_IS_DOUBLE;
    content_.asSlotId = extension.id();
    extension->asDouble = value;
  }
#else
  type_ = VALUE_IS_FLOAT;
  content_.asFloat = valueAsFloat;
#endif
  return true;
}

template <typename T>
enable_if_t<is_signed<T>::value> VariantData::setInteger(
    T value, ResourceManager* resources) {
  ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
  (void)resources;                             // silence warning

  if (canConvertNumber<int32_t>(value)) {
    type_ = VALUE_IS_INT32;
    content_.asInt32 = static_cast<int32_t>(value);
  }
#if ARDUINOJSON_USE_LONG_LONG
  else {
    auto extension = resources->allocExtension();
    if (extension) {
      type_ = VALUE_IS_INT64;
      content_.asSlotId = extension.id();
      extension->asInt64 = value;
    }
  }
#endif
}

template <typename T>
enable_if_t<is_unsigned<T>::value> VariantData::setInteger(
    T value, ResourceManager* resources) {
  ARDUINOJSON_ASSERT(type_ == VALUE_IS_NULL);  // must call clear() first
  (void)resources;                             // silence warning

  if (canConvertNumber<uint32_t>(value)) {
    type_ = VALUE_IS_UINT32;
    content_.asUint32 = static_cast<uint32_t>(value);
  }
#if ARDUINOJSON_USE_LONG_LONG
  else {
    auto extension = resources->allocExtension();
    if (extension) {
      type_ = VALUE_IS_UINT64;
      content_.asSlotId = extension.id();
      extension->asUint64 = value;
    }
  }
#endif
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
