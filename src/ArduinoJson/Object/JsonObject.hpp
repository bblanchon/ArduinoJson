// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectConst.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonArray;

class JsonObject : public VariantOperators<JsonObject> {
  friend class VariantAttorney;

 public:
  typedef JsonObjectIterator iterator;

  FORCE_INLINE JsonObject() : _data(0), _pool(0) {}
  FORCE_INLINE JsonObject(MemoryPool* buf, CollectionData* data)
      : _data(data), _pool(buf) {}

  operator VariantRef() const {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator JsonObjectConst() const {
    return JsonObjectConst(_data);
  }

  operator VariantConstRef() const {
    return VariantConstRef(collectionToVariant(_data));
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
    return iterator(_pool, _data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  void clear() const {
    if (!_data)
      return;
    _data->clear();
  }

  FORCE_INLINE bool set(JsonObjectConst src) {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }

  FORCE_INLINE bool operator==(JsonObject rhs) const {
    return JsonObjectConst(_data) == JsonObjectConst(rhs._data);
  }

  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<JsonObject, TString> >::type
  operator[](const TString& key) const {
    return MemberProxy<JsonObject, TString>(*this, key);
  }

  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<JsonObject, TChar*> >::type
  operator[](TChar* key) const {
    return MemberProxy<JsonObject, TChar*>(*this, key);
  }

  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it._slot);
  }

  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) const {
    removeMember(adaptString(key));
  }

  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    removeMember(adaptString(key));
  }

  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, bool>::type
  containsKey(const TString& key) const {
    return getMember(adaptString(key)) != 0;
  }

  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const {
    return getMember(adaptString(key)) != 0;
  }

  template <typename TString>
  FORCE_INLINE JsonArray createNestedArray(const TString& key) const;

  template <typename TChar>
  FORCE_INLINE JsonArray createNestedArray(TChar* key) const;

  template <typename TString>
  JsonObject createNestedObject(const TString& key) const {
    return operator[](key).template to<JsonObject>();
  }

  template <typename TChar>
  JsonObject createNestedObject(TChar* key) const {
    return operator[](key).template to<JsonObject>();
  }

 private:
  MemoryPool* getPool() const {
    return _pool;
  }

  VariantData* getData() const {
    return collectionToVariant(_data);
  }

  VariantData* getOrCreateData() const {
    return collectionToVariant(_data);
  }

  template <typename TAdaptedString>
  inline VariantData* getMember(TAdaptedString key) const {
    if (!_data)
      return 0;
    return _data->getMember(key);
  }

  template <typename TAdaptedString>
  void removeMember(TAdaptedString key) const {
    if (!_data)
      return;
    _data->removeMember(key);
  }

  CollectionData* _data;
  MemoryPool* _pool;
};

template <>
struct Converter<JsonObject> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonObject fromJson(VariantRef src) {
    VariantData* data = getData(src);
    MemoryPool* pool = getPool(src);
    return JsonObject(pool, data != 0 ? data->asObject() : 0);
  }

  static InvalidConversion<VariantConstRef, JsonObject> fromJson(
      VariantConstRef);

  static bool checkJson(VariantConstRef) {
    return false;
  }

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return data && data->isObject();
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
