// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayIterator.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ObjectRef;

class ArrayConstRef : public VariantOperators<ArrayConstRef> {
  friend class ArrayRef;
  friend class VariantAttorney;

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

  FORCE_INLINE ArrayConstRef() : _data(0) {}
  FORCE_INLINE ArrayConstRef(const CollectionData* data) : _data(data) {}

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

 private:
  const VariantData* getData() const {
    return collectionToVariant(_data);
  }

  const CollectionData* _data;
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

}  // namespace ARDUINOJSON_NAMESPACE
