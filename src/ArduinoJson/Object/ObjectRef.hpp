// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Object/ObjectConstRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ArrayRef;

class ObjectRef : public VariantOperators<ObjectRef> {
  friend class VariantAttorney;

 public:
  typedef ObjectIterator iterator;

  FORCE_INLINE ObjectRef() : _data(0), _pool(0) {}
  FORCE_INLINE ObjectRef(MemoryPool* buf, CollectionData* data)
      : _data(data), _pool(buf) {}

  operator VariantRef() const {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator ObjectConstRef() const {
    return ObjectConstRef(_data);
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

  FORCE_INLINE bool set(ObjectConstRef src) {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }

  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return ObjectConstRef(_data) == ObjectConstRef(rhs._data);
  }

  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<ObjectRef, TString> >::type
  operator[](const TString& key) const {
    return MemberProxy<ObjectRef, TString>(*this, key);
  }

  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<ObjectRef, TChar*> >::type
  operator[](TChar* key) const {
    return MemberProxy<ObjectRef, TChar*>(*this, key);
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
  FORCE_INLINE ArrayRef createNestedArray(const TString& key) const;

  template <typename TChar>
  FORCE_INLINE ArrayRef createNestedArray(TChar* key) const;

  template <typename TString>
  ObjectRef createNestedObject(const TString& key) const {
    return operator[](key).template to<ObjectRef>();
  }

  template <typename TChar>
  ObjectRef createNestedObject(TChar* key) const {
    return operator[](key).template to<ObjectRef>();
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
struct Converter<ObjectRef> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static ObjectRef fromJson(VariantRef src) {
    VariantData* data = getData(src);
    MemoryPool* pool = getPool(src);
    return ObjectRef(pool, data != 0 ? data->asObject() : 0);
  }

  static InvalidConversion<VariantConstRef, ObjectRef> fromJson(
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
