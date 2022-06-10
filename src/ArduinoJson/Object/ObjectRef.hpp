// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/ObjectFunctions.hpp>
#include <ArduinoJson/Object/ObjectIterator.hpp>

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))

namespace ARDUINOJSON_NAMESPACE {

template <typename TData>
class ObjectRefBase {
 public:
  operator VariantConstRef() const {
    return VariantConstRef(getVariantData());
  }

  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    return objectAccept(_data, visitor);
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
    return variantNesting(getVariantData());
  }

  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

 protected:
  const VariantData* getVariantData() const {
    const void* data = _data;  // prevent warning cast-align
    return reinterpret_cast<const VariantData*>(data);
  }

  ObjectRefBase(TData* data) : _data(data) {}
  TData* _data;
};

class ObjectConstRef : public ObjectRefBase<const CollectionData>,
                       public VariantOperators<ObjectConstRef>,
                       public Visitable {
  friend class ObjectRef;
  typedef ObjectRefBase<const CollectionData> base_type;

 public:
  typedef ObjectConstIterator iterator;

  ObjectConstRef() : base_type(0) {}
  ObjectConstRef(const CollectionData* data) : base_type(data) {}

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
    return !getMemberConst(key).isUnbound();
  }

  // containsKey(char*) const
  // containsKey(const char*) const
  // containsKey(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return !getMemberConst(key).isUnbound();
  }

  // getMemberConst(const std::string&) const
  // getMemberConst(const String&) const
  template <typename TString>
  FORCE_INLINE VariantConstRef getMemberConst(const TString& key) const {
    return VariantConstRef(objectGetMember(_data, adaptString(key)));
  }

  // getMemberConst(char*) const
  // getMemberConst(const char*) const
  // getMemberConst(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMemberConst(TChar* key) const {
    return VariantConstRef(objectGetMember(_data, adaptString(key)));
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return getMemberConst(key);
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return getMemberConst(key);
  }

  FORCE_INLINE bool operator==(ObjectConstRef rhs) const {
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
};

class ObjectRef : public ObjectRefBase<CollectionData>,
                  public ObjectShortcuts<ObjectRef>,
                  public VariantOperators<ObjectRef>,
                  public Visitable {
  typedef ObjectRefBase<CollectionData> base_type;

 public:
  typedef ObjectIterator iterator;

  FORCE_INLINE ObjectRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ObjectRef(MemoryPool* buf, CollectionData* data)
      : base_type(data), _pool(buf) {}

  operator VariantRef() const {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator ObjectConstRef() const {
    return ObjectConstRef(_data);
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

  // getMember(const std::string&) const
  // getMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getMember(const TString& key) const {
    return VariantRef(_pool, objectGetMember(_data, adaptString(key)));
  }

  // getMember(char*) const
  // getMember(const char*) const
  // getMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar* key) const {
    return VariantRef(_pool, objectGetMember(_data, adaptString(key)));
  }

  // getMemberConst(const std::string&) const
  // getMemberConst(const String&) const
  template <typename TString>
  FORCE_INLINE VariantConstRef getMemberConst(const TString& key) const {
    return VariantConstRef(objectGetMember(_data, adaptString(key)));
  }

  // getMemberConst(char*) const
  // getMemberConst(const char*) const
  // getMemberConst(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMemberConst(TChar* key) const {
    return VariantConstRef(objectGetMember(_data, adaptString(key)));
  }

  // getOrAddMember(const std::string&) const
  // getOrAddMember(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString& key) const {
    return VariantRef(_pool,
                      objectGetOrAddMember(_data, adaptString(key), _pool,
                                           getStringStoragePolicy(key)));
  }

  // getOrAddMember(char*) const
  // getOrAddMember(const char*) const
  // getOrAddMember(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar* key) const {
    return VariantRef(_pool,
                      objectGetOrAddMember(_data, adaptString(key), _pool,
                                           getStringStoragePolicy(key)));
  }

  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return ObjectConstRef(_data) == ObjectConstRef(rhs._data);
  }

  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it.internal());
  }

  // remove(const std::string&) const
  // remove(const String&) const
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) const {
    objectRemove(_data, adaptString(key));
  }

  // remove(char*) const
  // remove(const char*) const
  // remove(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    objectRemove(_data, adaptString(key));
  }

 private:
  MemoryPool* _pool;
};

template <>
struct Converter<ObjectConstRef> {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static ObjectConstRef fromJson(VariantConstRef src) {
    return ObjectConstRef(variantAsObject(getData(src)));
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->resolve()->isObject();
  }
};

template <>
struct Converter<ObjectRef> {
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
