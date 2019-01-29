// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "ObjectFunctions.hpp"
#include "ObjectIterator.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))

namespace ARDUINOJSON_NAMESPACE {

template <typename TData>
class ObjectRefBase {
 public:
  operator VariantConstRef() const {
    return VariantConstRef(reinterpret_cast<const VariantData*>(_data));
  }

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    objectAccept(_data, visitor);
  }

  // containsKey(const std::string&) const
  // containsKey(const String&) const
  template <typename TString>
  FORCE_INLINE bool containsKey(const TString& key) const {
    return objectContainsKey(_data, adaptString(key));
  }

  // containsKey(char*) const
  // containsKey(const char*) const
  // containsKey(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return objectContainsKey(_data, adaptString(key));
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }

  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

 protected:
  ObjectRefBase(TData* data) : _data(data) {}
  TData* _data;
};

class ObjectConstRef : public ObjectRefBase<const CollectionData>,
                       public Visitable {
  friend class ObjectRef;
  typedef ObjectRefBase<const CollectionData> base_type;

 public:
  typedef ObjectConstIterator iterator;

  ObjectConstRef() : base_type(0) {}
  ObjectConstRef(const CollectionData* data) : base_type(data) {}

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // get(const std::string&) const
  // get(const String&) const
  template <typename TString>
  FORCE_INLINE VariantConstRef get(const TString& key) const {
    return get_impl(adaptString(key));
  }

  // get(char*) const
  // get(const char*) const
  // get(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantConstRef get(TChar* key) const {
    return get_impl(adaptString(key));
  }

  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return get_impl(adaptString(key));
  }

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return get_impl(adaptString(key));
  }

  FORCE_INLINE bool operator==(ObjectConstRef rhs) const {
    return objectEquals(_data, rhs._data);
  }

 private:
  template <typename TAdaptedString>
  FORCE_INLINE VariantConstRef get_impl(TAdaptedString key) const {
    return VariantConstRef(objectGet(_data, key));
  }
};

class ObjectRef : public ObjectRefBase<CollectionData>,
                  public ObjectShortcuts<ObjectRef>,
                  public Visitable {
  typedef ObjectRefBase<CollectionData> base_type;

 public:
  typedef ObjectIterator iterator;

  FORCE_INLINE ObjectRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ObjectRef(MemoryPool* buf, CollectionData* data)
      : base_type(data), _pool(buf) {}

  operator VariantRef() const {
    return VariantRef(_pool, reinterpret_cast<VariantData*>(_data));
  }

  operator ObjectConstRef() const {
    return ObjectConstRef(_data);
  }

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_pool, _data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  void clear() const {
    if (!_data) return;
    _data->clear();
  }

  FORCE_INLINE bool copyFrom(ObjectConstRef src) {
    if (!_data || !src._data) return false;
    return _data->copyFrom(*src._data, _pool);
  }

  // get(const std::string&) const
  // get(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef get(const TString& key) const {
    return get_impl(adaptString(key));
  }

  // get(char*) const
  // get(const char*) const
  // get(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef get(TChar* key) const {
    return get_impl(adaptString(key));
  }

  // getOrCreate(const std::string&) const
  // getOrCreate(const String&) const
  template <typename TString>
  FORCE_INLINE VariantRef getOrCreate(const TString& key) const {
    return getOrCreate_impl(adaptString(key));
  }

  // getOrCreate(char*) const
  // getOrCreate(const char*) const
  // getOrCreate(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE VariantRef getOrCreate(TChar* key) const {
    return getOrCreate_impl(adaptString(key));
  }

  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return objectEquals(_data, rhs._data);
  }

  FORCE_INLINE void remove(iterator it) const {
    if (!_data) return;
    _data->remove(it.internal());
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
  template <typename TAdaptedString>
  FORCE_INLINE VariantRef get_impl(TAdaptedString key) const {
    return VariantRef(_pool, objectGet(_data, key));
  }

  template <typename TAdaptedString>
  FORCE_INLINE VariantRef getOrCreate_impl(TAdaptedString key) const {
    return VariantRef(_pool, objectGetOrCreate(_data, key, _pool));
  }

  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
