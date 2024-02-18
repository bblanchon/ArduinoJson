// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArrayIterator.hpp>
#include <ArduinoJson/Variant/VariantAttorney.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonObject;

// A read-only reference to an array in a JsonDocument
// https://arduinojson.org/v7/api/jsonarrayconst/
class JsonArrayConst : public detail::VariantOperators<JsonArrayConst> {
  friend class JsonArray;
  friend class detail::VariantAttorney;

 public:
  typedef JsonArrayConstIterator iterator;

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/begin/
  iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->createIterator(resources_), resources_);
  }

  // Returns an iterator to the element following the last element of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/end/
  iterator end() const {
    return iterator();
  }

  // Creates an unbound reference.
  JsonArrayConst() : data_(0) {}

  // INTERNAL USE ONLY
  JsonArrayConst(const detail::ArrayData* data,
                 const detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  // Returns the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarrayconst/subscript/
  JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(
        detail::ArrayData::getElement(data_, index, resources_), resources_);
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(getData(), resources_);
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonarrayconst/isnull/
  bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonarrayconst/isnull/
  operator bool() const {
    return data_ != 0;
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/nesting/
  size_t nesting() const {
    return detail::VariantData::nesting(getData(), resources_);
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/size/
  size_t size() const {
    return data_ ? data_->size(resources_) : 0;
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  const detail::VariantData* getData() const {
    return collectionToVariant(data_);
  }

  const detail::ArrayData* data_;
  const detail::ResourceManager* resources_;
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
