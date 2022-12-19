// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonObjectConst.hpp>
#include <ArduinoJson/Object/MemberProxy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonArray;

// A reference to an object in a JsonDocument.
// https://arduinojson.org/v6/api/jsonobject/
class JsonObject : public VariantOperators<JsonObject> {
  friend class VariantAttorney;

 public:
  typedef JsonObjectIterator iterator;

  // Creates an unbound reference.
  FORCE_INLINE JsonObject() : _data(0), _pool(0) {}

  // INTERNAL USE ONLY
  FORCE_INLINE JsonObject(MemoryPool* buf, CollectionData* data)
      : _data(data), _pool(buf) {}

  operator JsonVariant() const {
    void* data = _data;  // prevent warning cast-align
    return JsonVariant(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator JsonObjectConst() const {
    return JsonObjectConst(_data);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(_data));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonobject/isnull/
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonobject/isnull/
  FORCE_INLINE operator bool() const {
    return _data != 0;
  }

  // Returns the number of bytes occupied by the object.
  // https://arduinojson.org/v6/api/jsonobject/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the object.
  // https://arduinojson.org/v6/api/jsonobject/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(_data));
  }

  // Returns the number of members in the object.
  // https://arduinojson.org/v6/api/jsonobject/size/
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

  // Returns an iterator to the first key-value pair of the object.
  // https://arduinojson.org/v6/api/jsonobject/begin/
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_pool, _data->head());
  }

  // Returns an iterator following the last key-value pair of the object.
  // https://arduinojson.org/v6/api/jsonobject/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Removes all the members of the object.
  // ⚠️ Doesn't release the memory associated with the removed members.
  // https://arduinojson.org/v6/api/jsonobject/clear/
  void clear() const {
    if (!_data)
      return;
    _data->clear();
  }

  // Copies an object.
  // https://arduinojson.org/v6/api/jsonobject/set/
  FORCE_INLINE bool set(JsonObjectConst src) {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }

  // Compares the content of two objects.
  FORCE_INLINE bool operator==(JsonObject rhs) const {
    return JsonObjectConst(_data) == JsonObjectConst(rhs._data);
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobject/subscript/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<JsonObject, TString> >::type
  operator[](const TString& key) const {
    return MemberProxy<JsonObject, TString>(*this, key);
  }

  // Gets or sets the member with specified key.
  // https://arduinojson.org/v6/api/jsonobject/subscript/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<JsonObject, TChar*> >::type
  operator[](TChar* key) const {
    return MemberProxy<JsonObject, TChar*>(*this, key);
  }

  // Removes the member at the specified iterator.
  // ⚠️ Doesn't release the memory associated with the removed member.
  // https://arduinojson.org/v6/api/jsonobject/remove/
  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it._slot);
  }

  // Removes the member with the specified key.
  // ⚠️ Doesn't release the memory associated with the removed member.
  // https://arduinojson.org/v6/api/jsonobject/remove/
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) const {
    removeMember(adaptString(key));
  }

  // Removes the member with the specified key.
  // ⚠️ Doesn't release the memory associated with the removed member.
  // https://arduinojson.org/v6/api/jsonobject/remove/
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    removeMember(adaptString(key));
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, bool>::type
  containsKey(const TString& key) const {
    return getMember(adaptString(key)) != 0;
  }

  // Returns true if the object contains the specified key.
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value, bool>::type
  containsKey(TChar* key) const {
    return getMember(adaptString(key)) != 0;
  }

  // Creates an array and adds it to the object.
  // https://arduinojson.org/v6/api/jsonobject/createnestedarray/
  template <typename TString>
  FORCE_INLINE JsonArray createNestedArray(const TString& key) const;

  // Creates an array and adds it to the object.
  // https://arduinojson.org/v6/api/jsonobject/createnestedarray/
  template <typename TChar>
  FORCE_INLINE JsonArray createNestedArray(TChar* key) const;

  // Creates an object and adds it to the object.
  // https://arduinojson.org/v6/api/jsonobject/createnestedobject/
  template <typename TString>
  JsonObject createNestedObject(const TString& key) const {
    return operator[](key).template to<JsonObject>();
  }

  // Creates an object and adds it to the object.
  // https://arduinojson.org/v6/api/jsonobject/createnestedobject/
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
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonObject fromJson(JsonVariant src) {
    VariantData* data = getData(src);
    MemoryPool* pool = getPool(src);
    return JsonObject(pool, data != 0 ? data->asObject() : 0);
  }

  static InvalidConversion<JsonVariantConst, JsonObject> fromJson(
      JsonVariantConst);

  static bool checkJson(JsonVariantConst) {
    return false;
  }

  static bool checkJson(JsonVariant src) {
    VariantData* data = getData(src);
    return data && data->isObject();
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
