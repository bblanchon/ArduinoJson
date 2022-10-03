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
      return String(_content.asString.data, _content.asString.size,
                    String::Linked);
    case VALUE_IS_OWNED_STRING:
      return String(_content.asString.data, _content.asString.size,
                    String::Copied);
    default:
      return String();
  }
}

inline bool VariantData::copyFrom(const VariantData& src, MemoryPool* pool) {
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

template <typename TSource>
inline VariantRef VariantRefBase<TSource>::add() const {
  return VariantRef(getPool(), variantAddElement(getOrCreateData(), getPool()));
}

template <typename TSource>
inline VariantRef VariantRefBase<TSource>::getVariant() const {
  return VariantRef(getPool(), getData());
}

template <typename TSource>
inline VariantRef VariantRefBase<TSource>::getOrCreateVariant() const {
  return VariantRef(getPool(), getOrCreateData());
}

template <typename TSource>
template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRefBase<TSource>::to() const {
  return ArrayRef(getPool(), variantToArray(getOrCreateData()));
}

template <typename TSource>
template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRefBase<TSource>::to() const {
  return ObjectRef(getPool(), variantToObject(getOrCreateData()));
}

template <typename TSource>
template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRefBase<TSource>::to() const {
  variantSetNull(getOrCreateData());
  return *this;
}

// Out of class definition to avoid #1560
template <typename TSource>
inline bool VariantRefBase<TSource>::set(char value) const {
  return set(static_cast<signed char>(value));
}

template <typename TDataSource>
inline void convertToJson(const VariantRefBase<TDataSource>& src,
                          VariantRef dst) {
  dst.set(src.template as<VariantConstRef>());
}

// TODO: move somewhere else
template <typename TAdaptedString, typename TCallback>
bool CopyStringStoragePolicy::store(TAdaptedString str, MemoryPool* pool,
                                    TCallback callback) {
  const char* copy = pool->saveString(str);
  String storedString(copy, str.size(), String::Copied);
  callback(storedString);
  return copy != 0;
}

}  // namespace ARDUINOJSON_NAMESPACE
