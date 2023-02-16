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
  JsonObjectConst() : _data(0) {}

  // INTERNAL USE ONLY
  JsonObjectConst(const detail::CollectionData* data) : _data(data) {}

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(_data));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonobjectconst/isnull/
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonobjectconst/isnull/
  FORCE_INLINE operator bool() const {
    return _data != 0;
  }

  // Returns the number of bytes occupied by the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(_data));
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/size/
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v6/api/jsonobjectconst/begin/
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
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
    return getMember(detail::adaptString(key)) != 0;
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/containskey/
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return getMember(detail::adaptString(key)) != 0;
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/subscript/
  template <typename TString>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TString>::value,
                                          JsonVariantConst>::type
  operator[](const TString& key) const {
    return JsonVariantConst(getMember(detail::adaptString(key)));
  }

  // Gets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobjectconst/subscript/
  template <typename TChar>
  FORCE_INLINE typename detail::enable_if<detail::IsString<TChar*>::value,
                                          JsonVariantConst>::type
  operator[](TChar* key) const {
    return JsonVariantConst(getMember(detail::adaptString(key)));
  }

  // Compares objects.
  FORCE_INLINE bool operator==(JsonObjectConst rhs) const {
    if (_data == rhs._data)
      return true;

    if (!_data || !rhs._data)
      return false;

    size_t count = 0;
    for (iterator it = begin(); it != end(); ++it) {
      if (it->value() != rhs[it->key()])
        return false;
      count++;
    }
    return count == rhs.size();
  }

 private:
  const detail::VariantData* getData() const {
    return collectionToVariant(_data);
  }

  template <typename TAdaptedString>
  const detail::VariantData* getMember(TAdaptedString key) const {
    if (!_data)
      return 0;
    return _data->getMember(key);
  }

  const detail::CollectionData* _data;
};

template <>
struct Converter<JsonObjectConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
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
