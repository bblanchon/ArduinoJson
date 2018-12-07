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
  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    objectAccept(_data, visitor);
  }

  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE bool containsKey(const TKey& key) const {
    return objectContainsKey(_data, wrapString(key));
  }
  //
  // bool containsKey(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE bool containsKey(TKey* key) const {
    return objectContainsKey(_data, wrapString(key));
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
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

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, ArrayConstRef, ObjectConstRef
  template <typename TKey>
  FORCE_INLINE VariantConstRef get(const TKey& key) const {
    return get_impl(wrapString(key));
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const __FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, ArrayConstRef, ObjectConstRef
  template <typename TKey>
  FORCE_INLINE VariantConstRef get(TKey* key) const {
    return get_impl(wrapString(key));
  }

  //
  // VariantConstRef operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value, VariantConstRef>::type
  operator[](const TKey& key) const {
    return get_impl(wrapString(key));
  }
  //
  // VariantConstRef operator[](TKey) const;
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey*>::value, VariantConstRef>::type
  operator[](TKey* key) const {
    return get_impl(wrapString(key));
  }

  FORCE_INLINE bool operator==(ObjectConstRef rhs) const {
    return objectEquals(_data, rhs._data);
  }

 private:
  template <typename TKey>
  FORCE_INLINE VariantConstRef get_impl(TKey key) const {
    return VariantConstRef(objectGet(_data, key));
  }
};

class ObjectRef : public ObjectRefBase<CollectionData>, public Visitable {
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

  // Creates and adds a ArrayRef.
  //
  // ArrayRef createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(const TKey& key) const;
  // ArrayRef createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(TKey* key) const;

  // Creates and adds a ObjectRef.
  //
  // ObjectRef createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE ObjectRef createNestedObject(const TKey& key) const {
    return set(key).template to<ObjectRef>();
  }
  //
  // ObjectRef createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE ObjectRef createNestedObject(TKey* key) const {
    return set(key).template to<ObjectRef>();
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TKey>
  FORCE_INLINE VariantRef get(const TKey& key) const {
    return get_impl(wrapString(key));
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const __FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TKey>
  FORCE_INLINE VariantRef get(TKey* key) const {
    return get_impl(wrapString(key));
  }

  // Gets or sets the value associated with the specified key.
  //
  // ObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE ObjectSubscript<const TKey&> operator[](const TKey& key) const {
    return ObjectSubscript<const TKey&>(*this, key);
  }
  //
  // ObjectSubscript operator[](TKey)
  // TKey = char*, const char*, char[], const char[N], const
  // __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE ObjectSubscript<TKey*> operator[](TKey* key) const {
    return ObjectSubscript<TKey*>(*this, key);
  }

  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return objectEquals(_data, rhs._data);
  }

  FORCE_INLINE void remove(iterator it) const {
    if (!_data) return;
    _data->remove(it.internal());
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE void remove(const TKey& key) const {
    objectRemove(_data, wrapString(key));
  }
  //
  // void remove(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE void remove(TKey* key) const {
    objectRemove(_data, wrapString(key));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef set(TKey* key) const {
    return set_impl(wrapString(key));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef set(const TKey& key) const {
    return set_impl(wrapString(key));
  }

 private:
  template <typename TKey>
  FORCE_INLINE VariantRef get_impl(TKey key) const {
    return VariantRef(_pool, objectGet(_data, key));
  }

  template <typename TKey>
  FORCE_INLINE VariantRef set_impl(TKey key) const {
    return VariantRef(_pool, objectSet(_data, key, _pool));
  }

  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
