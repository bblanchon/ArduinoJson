// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayFunctions.hpp>
#include <ArduinoJson/Array/ArrayIterator.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

// Returns the size (in bytes) of an array with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))

namespace ARDUINOJSON_NAMESPACE {

class ObjectRef;

template <typename TData>
class ArrayRefBase {
  friend class VariantAttorney;

 public:
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

 protected:
  const VariantData* getData() const {
    return collectionToVariant(_data);
  }

  ArrayRefBase(TData* data) : _data(data) {}
  TData* _data;
};

class ArrayConstRef : public ArrayRefBase<const CollectionData>,
                      public VariantOperators<ArrayConstRef> {
  friend class ArrayRef;
  typedef ArrayRefBase<const CollectionData> base_type;

 public:
  typedef ArrayConstRefIterator iterator;

  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  FORCE_INLINE ArrayConstRef() : base_type(0) {}
  FORCE_INLINE ArrayConstRef(const CollectionData* data) : base_type(data) {}

  FORCE_INLINE bool operator==(ArrayConstRef rhs) const {
    if (_data == rhs._data)
      return true;
    if (!_data || !rhs._data)
      return false;

    iterator it1 = begin();
    iterator it2 = rhs.begin();

    for (;;) {
      bool end1 = it1 == end();
      bool end2 = it2 == rhs.end();
      if (end1 && end2)
        return true;
      if (end1 || end2)
        return false;
      if (*it1 != *it2)
        return false;
      ++it1;
      ++it2;
    }
  }

  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return VariantConstRef(_data ? _data->getElement(index) : 0);
  }
};

class ArrayRef : public ArrayRefBase<CollectionData>,
                 public ArrayShortcuts<ArrayRef>,
                 public VariantOperators<ArrayRef> {
  typedef ArrayRefBase<CollectionData> base_type;

  friend class VariantAttorney;

 public:
  typedef ArrayIterator iterator;

  FORCE_INLINE ArrayRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ArrayRef(MemoryPool* pool, CollectionData* data)
      : base_type(data), _pool(pool) {}

  operator VariantRef() {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator ArrayConstRef() const {
    return ArrayConstRef(_data);
  }

  VariantRef add() const {
    return VariantRef(_pool, arrayAdd(_data, _pool));
  }

  using ArrayShortcuts<ArrayRef>::add;

  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_pool, _data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Copy a ArrayRef
  FORCE_INLINE bool set(ArrayConstRef src) const {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }

  FORCE_INLINE bool operator==(ArrayRef rhs) const {
    return ArrayConstRef(_data) == ArrayConstRef(rhs._data);
  }

  // Removes element at specified position.
  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it.internal());
  }

  // Removes element at specified index.
  FORCE_INLINE void remove(size_t index) const {
    if (!_data)
      return;
    _data->removeElement(index);
  }

  void clear() const {
    if (!_data)
      return;
    _data->clear();
  }

 protected:
  MemoryPool* getPool() const {
    return _pool;
  }

  VariantData* getData() const {
    return collectionToVariant(_data);
  }

  VariantData* getOrCreateData() const {
    return collectionToVariant(_data);
  }

 private:
  MemoryPool* _pool;
};

template <>
struct Converter<ArrayConstRef> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static ArrayConstRef fromJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data ? data->asArray() : 0;
  }

  static bool checkJson(VariantConstRef src) {
    const VariantData* data = getData(src);
    return data && data->isArray();
  }
};

template <>
struct Converter<ArrayRef> : private VariantAttorney {
  static void toJson(VariantConstRef src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static ArrayRef fromJson(VariantRef src) {
    VariantData* data = getData(src);
    MemoryPool* pool = getPool(src);
    return ArrayRef(pool, data != 0 ? data->asArray() : 0);
  }

  static InvalidConversion<VariantConstRef, ArrayRef> fromJson(VariantConstRef);

  static bool checkJson(VariantConstRef) {
    return false;
  }

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return data && data->isArray();
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
