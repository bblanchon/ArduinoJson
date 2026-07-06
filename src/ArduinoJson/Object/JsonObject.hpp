// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
  using iterator = JsonObjectIterator;

  // Creates an unbound reference.
  JsonObject() {}

  // INTERNAL USE ONLY
  JsonObject(const detail::VariantImpl& impl) : impl_(impl) {}

  // INTERNAL USE ONLY
  JsonObject(detail::VariantData* data, detail::ResourceManager* resource)
      : impl_(data, resource) {}

  operator JsonVariant() const {
    return JsonVariant(getData(), getResourceManager());
  }

  operator JsonObjectConst() const {
    return JsonObjectConst(getData(), getResourceManager());
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(getData(), getResourceManager());
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonobject/isnull/
  bool isNull() const {
    return !impl_.isObject();
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonobject/isnull/
  operator bool() const {
    return impl_.isObject();
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v7/api/jsonobject/nesting/
  size_t nesting() const {
    return impl_.nesting();
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v7/api/jsonobject/size/
  size_t size() const {
    return impl_.size();
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobject/begin/
  iterator begin() const {
    return iterator(impl_.createIterator(), impl_.resources());
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobject/end/
  iterator end() const {
    return iterator();
  }

  // Removes all the members of the object.
  // https://arduinojson.org/v7/api/jsonobject/clear/
  void clear() const {
    impl_.empty();
  }

  // Copies an object.
  // https://arduinojson.org/v7/api/jsonobject/set/
  bool set(JsonObjectConst src) {
    if (isNull() || src.isNull())
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
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  detail::MemberProxy<JsonObject, detail::AdaptedString<TString>> operator[](
      const TString& key) const {
    return {*this, detail::adaptString(key)};
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobject/subscript/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  detail::MemberProxy<JsonObject, detail::AdaptedString<TChar*>> operator[](
      TChar* key) const {
    return {*this, detail::adaptString(key)};
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobject/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  detail::MemberProxy<JsonObject, detail::AdaptedString<JsonString>> operator[](
      const TVariant& key) const {
    return {*this, detail::adaptString(key.template as<JsonString>())};
  }

  // Removes the member at the specified iterator.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  FORCE_INLINE void remove(iterator it) const {
    impl_.removeMember(it.iterator_);
  }

  // Removes the member with the specified key.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  void remove(const TString& key) const {
    impl_.removeMember(detail::adaptString(key));
  }

  // Removes the member with the specified key.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  void remove(const TVariant& key) const {
    if (key.template is<const char*>())
      remove(key.template as<const char*>());
  }

  // Removes the member with the specified key.
  // https://arduinojson.org/v7/api/jsonobject/remove/
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    impl_.removeMember(detail::adaptString(key));
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobject/containskey/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TString& key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobject/containskey/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobject/containskey/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TVariant& key) const {
    return containsKey(key.template as<const char*>());
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
    return impl_.resources();
  }

  detail::VariantData* getData() const {
    return impl_.data();
  }

  detail::VariantData* getOrCreateData() const {
    return impl_.data();
  }

  mutable detail::VariantImpl impl_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
