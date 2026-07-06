// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
  using iterator = JsonObjectConstIterator;

  // Creates an unbound reference.
  JsonObjectConst() {}

  // INTERNAL USE ONLY
  JsonObjectConst(detail::VariantData* data, detail::ResourceManager* resources)
      : impl_(data, resources) {}

  // INTERNAL USE ONLY
  JsonObjectConst(const detail::VariantImpl& impl) : impl_(impl) {}

  operator JsonVariantConst() const {
    return JsonVariantConst(impl_.data(), impl_.resources());
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonobjectconst/isnull/
  bool isNull() const {
    return impl_.isNull();
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonobjectconst/isnull/
  operator bool() const {
    return !isNull();
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/nesting/
  size_t nesting() const {
    return impl_.nesting();
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/size/
  size_t size() const {
    return impl_.size();
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/begin/
  iterator begin() const {
    return iterator(impl_.createIterator(), impl_.resources());
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v7/api/jsonobjectconst/end/
  iterator end() const {
    return iterator();
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TString& key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj["key"].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TChar>
  ARDUINOJSON_DEPRECATED("use obj[\"key\"].is<T>() instead")
  bool containsKey(TChar* key) const {
    return impl_.getMember(detail::adaptString(key)) != 0;
  }

  // DEPRECATED: use obj[key].is<T>() instead
  // https://arduinojson.org/v7/api/jsonobjectconst/containskey/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  ARDUINOJSON_DEPRECATED("use obj[key].is<T>() instead")
  bool containsKey(const TVariant& key) const {
    return containsKey(key.template as<const char*>());
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TString,
            detail::enable_if_t<detail::IsString<TString>::value, int> = 0>
  JsonVariantConst operator[](const TString& key) const {
    return JsonVariantConst(impl_.getMember(detail::adaptString(key)),
                            impl_.resources());
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TChar,
            detail::enable_if_t<detail::IsString<TChar*>::value, int> = 0>
  JsonVariantConst operator[](TChar* key) const {
    return JsonVariantConst(impl_.getMember(detail::adaptString(key)),
                            impl_.resources());
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v7/api/jsonobjectconst/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  JsonVariantConst operator[](const TVariant& key) const {
    if (key.template is<JsonString>())
      return operator[](key.template as<JsonString>());
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
    return impl_.data();
  }

  detail::VariantImpl impl_;
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
