// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectConst.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonArray;

// A reference to an object in a JsonDocument.
// https://arduinojson.org/v7/api/jsonobject/
class JsonObject : public detail::VariantOperators<JsonObject> {
  friend class detail::VariantAttorney;

 public:
  typedef JsonObjectIterator iterator;

  // Creates an unbound reference.
  JsonObject() : data_(0), resources_(0) {}

  // INTERNAL USE ONLY
  JsonObject(detail::ObjectData* data, detail::ResourceManager* resource)
      : data_(data), resources_(resource) {}

  operator JsonVariant() const {
    void* data = data_;  // prevent warning cast-align
    return JsonVariant(reinterpret_cast<detail::VariantData*>(data),
                       resources_);
  }

  operator JsonObjectConst() const {
    return JsonObjectConst(data_, resources_);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_), resources_);
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonobject/isnull/
  bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonobject/isnull/
  operator bool() const {
    return data_ != 0;
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v7/api/jsonobject/nesting/
  size_t nesting() const {
    return detail::VariantData::nesting(collectionToVariant(data_), resources_);
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v7/api/jsonobject/size/
  size_t size() const {
    return data_ ? data_->size(resources_) : 0;
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobject/begin/
  iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->createIterator(resources_), resources_);
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobject/end/
  iterator end() const {
    return iterator();
  }

  // Removes all the members of the object.
  // https://arduinojson.org/v7/api/jsonobject/clear/
  void clear() const {
    detail::ObjectData::clear(data_, resources_);
  }

  // Copies an object.
  // https://arduinojson.org/v7/api/jsonobject/set/
  bool set(JsonObjectConst src) {
    if (!data_ || !src.data_)
      return false;

    clear();
    for (auto kvp : src) {
      if (!operator[](kvp.key()).set(kvp.value()))
        return false;
    }

    return true;
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobject/subscript/
  template <typename TString>

  typename detail::enable_if<detail::IsString<TString>::value,
                             detail::MemberProxy<JsonObject, TString>>::type
  operator[](const TString& key) const {
    return {*this, key};
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobject/subscript/
  template <typename TChar>

  typename detail::enable_if<detail::IsString<TChar*>::value,
                             detail::MemberProxy<JsonObject, TChar*>>::type
  operator[](TChar* key) const {
    return {*this, key};
  }

  // Removes the member at the specified iterator.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  FORCE_INLINE void remove(iterator it) const {
    detail::ObjectData::remove(data_, it.iterator_, resources_);
  }

  // Removes the member with the specified key.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) const {
    detail::ObjectData::removeMember(data_, detail::adaptString(key),
                                     resources_);
  }

  // Removes the member with the specified key.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    detail::ObjectData::removeMember(data_, detail::adaptString(key),
                                     resources_);
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v7/api/jsonobject/containskey/
  template <typename TString>

  typename detail::enable_if<detail::IsString<TString>::value, bool>::type
  containsKey(const TString& key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key),
                                         resources_) != 0;
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v7/api/jsonobject/containskey/
  template <typename TChar>

  typename detail::enable_if<detail::IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key),
                                         resources_) != 0;
  }

  // DEPRECATED: use obj[key].to<JsonArray>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use obj[key].to<JsonArray>() instead")
  JsonArray createNestedArray(TChar* key) const {
    return operator[](key).template to<JsonArray>();
  }

  // DEPRECATED: use obj[key].to<JsonArray>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use obj[key].to<JsonArray>() instead")
  JsonArray createNestedArray(const TString& key) const {
    return operator[](key).template to<JsonArray>();
  }

  // DEPRECATED: use obj[key].to<JsonObject>() instead
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use obj[key].to<JsonObject>() instead")
  JsonObject createNestedObject(TChar* key) {
    return operator[](key).template to<JsonObject>();
  }

  // DEPRECATED: use obj[key].to<JsonObject>() instead
  template <typename TString>
  ARDUINOJSON_DEPRECATED("use obj[key].to<JsonObject>() instead")
  JsonObject createNestedObject(const TString& key) {
    return operator[](key).template to<JsonObject>();
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  detail::ResourceManager* getResourceManager() const {
    return resources_;
  }

  detail::VariantData* getData() const {
    return detail::collectionToVariant(data_);
  }

  detail::VariantData* getOrCreateData() const {
    return detail::collectionToVariant(data_);
  }

  detail::ObjectData* data_;
  detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
