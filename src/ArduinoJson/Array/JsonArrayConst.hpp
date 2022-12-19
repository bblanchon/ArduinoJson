// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArrayIterator.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonObject;

// A read-only reference to an array in a JsonDocument
// https://arduinojson.org/v6/api/jsonarrayconst/
class JsonArrayConst : public VariantOperators<JsonArrayConst> {
  friend class JsonArray;
  friend class VariantAttorney;

 public:
  typedef JsonArrayConstIterator iterator;

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/begin/
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
  }

  // Returns an iterator to the element following the last element of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Creates an unbound reference.
  FORCE_INLINE JsonArrayConst() : _data(0) {}

  // INTERNAL USE ONLY
  FORCE_INLINE JsonArrayConst(const CollectionData* data) : _data(data) {}

  // Compares the content of two arrays.
  // Returns true if the two arrays are equal.
  FORCE_INLINE bool operator==(JsonArrayConst rhs) const {
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

  // Returns the element at the specified index.
  // https://arduinojson.org/v6/api/jsonarrayconst/subscript/
  FORCE_INLINE JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(_data ? _data->getElement(index) : 0);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(_data));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonarrayconst/isnull/
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonarrayconst/isnull/
  FORCE_INLINE operator bool() const {
    return _data != 0;
  }

  // Returns the number of bytes occupied by the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(_data));
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/size/
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
struct Converter<JsonArrayConst> : private VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonArrayConst fromJson(JsonVariantConst src) {
    const VariantData* data = getData(src);
    return data ? data->asArray() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    const VariantData* data = getData(src);
    return data && data->isArray();
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
