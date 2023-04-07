// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArrayIterator.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonObject;

// A read-only reference to an array in a JsonDocument
// https://arduinojson.org/v6/api/jsonarrayconst/
class JsonArrayConst : public detail::VariantOperators<JsonArrayConst> {
  friend class JsonArray;
  friend class detail::VariantAttorney;

 public:
  typedef JsonArrayConstIterator iterator;

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/begin/
  FORCE_INLINE iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->head());
  }

  // Returns an iterator to the element following the last element of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Creates an unbound reference.
  FORCE_INLINE JsonArrayConst() : data_(0) {}

  // INTERNAL USE ONLY
  FORCE_INLINE JsonArrayConst(const detail::CollectionData* data)
      : data_(data) {}

  // Compares the content of two arrays.
  // Returns true if the two arrays are equal.
  FORCE_INLINE bool operator==(JsonArrayConst rhs) const {
    if (data_ == rhs.data_)
      return true;
    if (!data_ || !rhs.data_)
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
    return JsonVariantConst(data_ ? data_->getElement(index) : 0);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonarrayconst/isnull/
  FORCE_INLINE bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonarrayconst/isnull/
  FORCE_INLINE operator bool() const {
    return data_ != 0;
  }

  // Returns the number of bytes occupied by the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return data_ ? data_->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(data_));
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/size/
  FORCE_INLINE size_t size() const {
    return data_ ? data_->size() : 0;
  }

 private:
  const detail::VariantData* getData() const {
    return collectionToVariant(data_);
  }

  const detail::CollectionData* data_;
};

template <>
struct Converter<JsonArrayConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonArrayConst fromJson(JsonVariantConst src) {
    auto data = getData(src);
    return data ? data->asArray() : 0;
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isArray();
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
