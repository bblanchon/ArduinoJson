// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

#include <string.h>  // for strcmp

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return _content.asBoolean;
    case VALUE_IS_UNSIGNED_INTEGER:
      return convertNumber<T>(_content.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return convertNumber<T>(_content.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString.data);
    case VALUE_IS_FLOAT:
      return convertNumber<T>(_content.asFloat);
    default:
      return 0;
  }
}

inline bool VariantData::asBoolean() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return _content.asBoolean;
    case VALUE_IS_SIGNED_INTEGER:
    case VALUE_IS_UNSIGNED_INTEGER:
      return _content.asUnsignedInteger != 0;
    case VALUE_IS_FLOAT:
      return _content.asFloat != 0;
    case VALUE_IS_NULL:
      return false;
    default:
      return true;
  }
}

// T = float/double
template <typename T>
inline T VariantData::asFloat() const {
  switch (type()) {
    case VALUE_IS_BOOLEAN:
      return static_cast<T>(_content.asBoolean);
    case VALUE_IS_UNSIGNED_INTEGER:
      return static_cast<T>(_content.asUnsignedInteger);
    case VALUE_IS_SIGNED_INTEGER:
      return static_cast<T>(_content.asSignedInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString.data);
    case VALUE_IS_FLOAT:
      return static_cast<T>(_content.asFloat);
    default:
      return 0;
  }
}

inline String VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
      return String(_content.asString.data, _content.asString.size, true);
    case VALUE_IS_OWNED_STRING:
      return String(_content.asString.data, _content.asString.size, false);
    default:
      return String();
  }
}

inline bool VariantData::copyFrom(const VariantData &src, MemoryPool *pool) {
  switch (src.type()) {
    case VALUE_IS_ARRAY:
      return toArray().copyFrom(src._content.asCollection, pool);
    case VALUE_IS_OBJECT:
      return toObject().copyFrom(src._content.asCollection, pool);
    case VALUE_IS_OWNED_STRING: {
      String value = src.asString();
      return storeString(adaptString(value), pool,
                         getStringStoragePolicy(value));
    }
    case VALUE_IS_OWNED_RAW:
      return storeOwnedRaw(
          serialized(src._content.asString.data, src._content.asString.size),
          pool);
    default:
      setType(src.type());
      _content = src._content;
      return true;
  }
}

template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRef::to() const {
  return ArrayRef(_pool, variantToArray(_data));
}

template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRef::to() const {
  return ObjectRef(_pool, variantToObject(_data));
}

template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRef::to() const {
  variantSetNull(_data);
  return *this;
}

inline VariantConstRef VariantConstRef::getElement(size_t index) const {
  return ArrayConstRef(_data != 0 ? _data->asArray() : 0)[index];
}

inline VariantRef VariantRef::addElement() const {
  return VariantRef(_pool, variantAddElement(_data, _pool));
}

inline VariantRef VariantRef::getElement(size_t index) const {
  return VariantRef(_pool, _data != 0 ? _data->getElement(index) : 0);
}

inline VariantRef VariantRef::getOrAddElement(size_t index) const {
  return VariantRef(_pool, variantGetOrAddElement(_data, index, _pool));
}

template <typename TChar>
inline VariantRef VariantRef::getMember(TChar *key) const {
  return VariantRef(_pool, _data != 0 ? _data->getMember(adaptString(key)) : 0);
}

template <typename TString>
inline typename enable_if<IsString<TString>::value, VariantRef>::type
VariantRef::getMember(const TString &key) const {
  return VariantRef(_pool, _data != 0 ? _data->getMember(adaptString(key)) : 0);
}

template <typename TChar>
inline VariantRef VariantRef::getOrAddMember(TChar *key) const {
  return VariantRef(_pool, variantGetOrAddMember(_data, key, _pool));
}

template <typename TString>
inline VariantRef VariantRef::getOrAddMember(const TString &key) const {
  return VariantRef(_pool, variantGetOrAddMember(_data, key, _pool));
}

inline VariantConstRef operator|(VariantConstRef preferedValue,
                                 VariantConstRef defaultValue) {
  return preferedValue ? preferedValue : defaultValue;
}

// Out of class definition to avoid #1560
inline bool VariantRef::set(char value) const {
  return set<signed char>(value);
}

// TODO: move somewhere else
template <typename TAdaptedString, typename TCallback>
bool CopyStringStoragePolicy::store(TAdaptedString str, MemoryPool *pool,
                                    TCallback callback) {
  const char *copy = pool->saveString(str);
  CopiedString storedString(copy, str.size());
  callback(storedString);
  return copy != 0;
}

}  // namespace ARDUINOJSON_NAMESPACE
