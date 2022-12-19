// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectIterator.hpp>
#include <ArduinoJson/Variant/VariantOperators.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonObjectConst : public VariantOperators<JsonObjectConst> {
  friend class JsonObject;
  friend class VariantAttorney;

 public:
  typedef JsonObjectConstIterator iterator;

  JsonObjectConst() : _data(0) {}
  JsonObjectConst(const CollectionData* data) : _data(data) {}

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(_data));
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  FORCE_INLINE operator bool() const {
    return _data != 0;
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(_data));
  }

  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // containsKey(const std::string&) const
  // containsKey(const String&) const
  template <typename TString>
  FORCE_INLINE bool containsKey(const TString& key) const {
    return getMember(adaptString(key)) != 0;
  }

  // containsKey(char*) const
  // containsKey(const char*) const
  // containsKey(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return getMember(adaptString(key)) != 0;
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, JsonVariantConst>::type
      operator[](const TString& key) const {
    return JsonVariantConst(getMember(adaptString(key)));
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, JsonVariantConst>::type
      operator[](TChar* key) const {
    return JsonVariantConst(getMember(adaptString(key)));
  }

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
  const VariantData* getData() const {
    return collectionToVariant(_data);
  }

  template <typename TAdaptedString>
  const VariantData* getMember(TAdaptedString key) const {
    if (!_data)
      return 0;
    return _data->getMember(key);
  }

  const CollectionData* _data;
};

template <>
struct Converter<JsonObjectConst> : private VariantAttorney {
  static void toJson(JsonVariantConst src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonObjectConst fromJson(JsonVariantConst src) {
    const VariantData* data = getData(src);
    return data != 0 ? data->asObject() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    const VariantData* data = getData(src);
    return data && data->isObject();
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
