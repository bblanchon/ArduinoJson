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
    return iterator(data_->createIterator(), resources_);
  }

  // Returns an iterator to the element following the last element of the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Creates an unbound reference.
  FORCE_INLINE JsonArrayConst() : data_(0) {}

  // INTERNAL USE ONLY
  FORCE_INLINE JsonArrayConst(const detail::ArrayData* data,
                              const detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  // Returns the element at the specified index.
  // https://arduinojson.org/v6/api/jsonarrayconst/subscript/
  FORCE_INLINE JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(detail::ArrayData::getElement(data_, index),
                            resources_);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_), resources_);
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
    return detail::VariantData::nesting(collectionToVariant(data_));
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

  const detail::ArrayData* data_;
  const detail::ResourceManager* resources_;
};

template <>
struct Converter<JsonArrayConst> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    detail::VariantData::copy(getData(dst), getData(src),
                              getResourceManager(dst));
  }

  static JsonArrayConst fromJson(JsonVariantConst src) {
    auto data = getData(src);
    auto array = data ? data->asArray() : nullptr;
    return JsonArrayConst(array, getResourceManager(src));
  }

  static bool checkJson(JsonVariantConst src) {
    auto data = getData(src);
    return data && data->isArray();
  }
};

// Compares the content of two arrays.
// Returns true if the two arrays are equal.
inline bool operator==(JsonArrayConst lhs, JsonArrayConst rhs) {
  if (!lhs && !rhs)
    return true;
  if (!lhs || !rhs)
    return false;

  auto a = lhs.begin();
  auto b = rhs.begin();

  for (;;) {
    if (a == b)  // same pointer or both null
      return true;
    if (a == lhs.end() || b == rhs.end())
      return false;
    if (*a != *b)
      return false;
    ++a;
    ++b;
  }
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
