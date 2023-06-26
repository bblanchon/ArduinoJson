// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectIterator.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A read-only reference to an object in a JsonDocument.
// https://arduinojson.org/v6/api/jsonobjectconst/
class JsonObjectConst : public detail::VariantOperators<JsonObjectConst> {
  friend class JsonObject;
  friend class detail::VariantAttorney;

 public:
  typedef JsonObjectConstIterator iterator;

  // Creates an unbound reference.
  JsonObjectConst() : data_(0) {}

  // INTERNAL USE ONLY
  JsonObjectConst(const detail::ObjectData* data) : data_(data) {}

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonobjectconst/isnull/
  FORCE_INLINE bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonobjectconst/isnull/
  FORCE_INLINE operator bool() const {
    return data_ != 0;
  }

  // Returns the number of bytes occupied by the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return data_ ? data_->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return detail::VariantData::nesting(collectionToVariant(data_));
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/size/
  FORCE_INLINE size_t size() const {
    return data_ ? data_->size() : 0;
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/begin/
  FORCE_INLINE iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->head());
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/containskey/
  template <typename TString>
  FORCE_INLINE bool containsKey(const TString& key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key)) != 0;
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/containskey/
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return detail::ObjectData::getMember(data_, detail::adaptString(key)) != 0;
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/subscript/
  template <typename TString>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TString>::value,
                                          JsonVariantConst>::type
  operator[](const TString& key) const {
    return JsonVariantConst(
        detail::ObjectData::getMember(data_, detail::adaptString(key)));
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/subscript/
  template <typename TChar>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TChar*>::value,
                                          JsonVariantConst>::type
  operator[](TChar* key) const {
    return JsonVariantConst(
        detail::ObjectData::getMember(data_, detail::adaptString(key)));
  }

  // Compares objects.
  FORCE_INLINE bool operator==(JsonObjectConst rhs) const {
    return detail::ObjectData::equals(data_, rhs.data_);
  }

 private:
  const detail::VariantData* getData() const {
    return collectionToVariant(data_);
  }

  const detail::ObjectData* data_;
};

template <>
struct Converter<JsonObjectConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    detail::VariantData::copy(getData(dst), getData(src),
                              getResourceManager(dst));
  }

  static JsonObjectConst fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data != 0 ? data->asObject() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isObject();
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
