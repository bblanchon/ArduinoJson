// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectIterator.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A read-only reference to an object in a JsonDocument.
// https://arduinojson.org/v7/api/jsonobjectconst/
class JsonObjectConst : public detail::VariantOperators<JsonObjectConst> {
  friend class JsonObject;
  friend class detail::VariantAttorney;

 public:
  typedef JsonObjectConstIterator iterator;

  // Creates an unbound reference.
  JsonObjectConst() : data_(0), resources_(0) {}

  // INTERNAL USE ONLY
  JsonObjectConst(const detail::ObjectData* data,
                  const detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  operator JsonVariantConst() const {
    return JsonVariantConst(getData(), resources_);
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonobjectconst/isnull/
  bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonobjectconst/isnull/
  operator bool() const {
    return data_ != 0;
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/nesting/
  size_t nesting() const {
    return detail::VariantData::nesting(getData(), resources_);
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/size/
  size_t size() const {
    return data_ ? data_->size(resources_) : 0;
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/begin/
  iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->createIterator(resources_), resources_);
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/end/
  iterator end() const {
    return iterator();
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  detail::enable_if_t<detail::IsString<TString>::value, bool> containsKey(
      const TString& key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key),
                                         resources_) != 0;
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key),
                                         resources_) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TVariant>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  detail::enable_if_t<detail::IsVariant<TVariant>::value, bool> containsKey(
      const TVariant& key) const {
    return containsKey(key.template as<const char*>());
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TString>
  detail::enable_if_t<detail::IsString<TString>::value, JsonVariantConst>
  operator[](const TString& key) const {
    return JsonVariantConst(detail::ObjectData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TChar>
  detail::enable_if_t<detail::IsString<TChar*>::value, JsonVariantConst>
  operator[](TChar* key) const {
    return JsonVariantConst(detail::ObjectData::getMember(
                                data_, detail::adaptString(key), resources_),
                            resources_);
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TVariant>
  detail::enable_if_t<detail::IsVariant<TVariant>::value, JsonVariantConst>
  operator[](const TVariant& key) const {
    if (key.template is<const char*>())
      return operator[](key.template as<const char*>());
    else
      return JsonVariantConst();
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  const detail::VariantData* getData() const {
    return collectionToVariant(data_);
  }

  const detail::ObjectData* data_;
  const detail::ResourceManager* resources_;
};

inline bool operator==(JsonObjectConst lhs, JsonObjectConst rhs) {
  if (!lhs && !rhs)  // both are null
    return true;

  if (!lhs || !rhs)  // only one is null
    return false;

  size_t count = 0;
  for (auto kvp : lhs) {
    auto rhsValue = rhs[kvp.key()];
    if (rhsValue.isUnbound())
      return false;
    if (kvp.value() != rhsValue)
      return false;
    count++;
  }
  return count == rhs.size();
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
