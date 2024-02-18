// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Array/JsonArrayConst.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonObject;

// A reference to an array in a JsonDocument
// https://arduinojson.org/v7/api/jsonarray/
class JsonArray : public detail::VariantOperators<JsonArray> {
  friend class detail::VariantAttorney;

 public:
  typedef JsonArrayIterator iterator;

  // Constructs an unbound reference.
  JsonArray() : data_(0), resources_(0) {}

  // INTERNAL USE ONLY
  JsonArray(detail::ArrayData* data, detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  // Returns a JsonVariant pointing to the array.
  // https://arduinojson.org/v7/api/jsonvariant/
  operator JsonVariant() {
    void* data = data_;  // prevent warning cast-align
    return JsonVariant(reinterpret_cast<detail::VariantData*>(data),
                       resources_);
  }

  // Returns a read-only reference to the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/
  operator JsonArrayConst() const {
    return JsonArrayConst(data_, resources_);
  }

  // Appends a new (empty) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T>
  typename detail::enable_if<!detail::is_same<T, JsonVariant>::value, T>::type
  add() const {
    return add<JsonVariant>().to<T>();
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T>
  typename detail::enable_if<detail::is_same<T, JsonVariant>::value, T>::type
  add() const {
    return JsonVariant(detail::ArrayData::addElement(data_, resources_),
                       resources_);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T>
  bool add(const T& value) const {
    return add<JsonVariant>().set(value);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T>
  bool add(T* value) const {
    return add<JsonVariant>().set(value);
  }

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v7/api/jsonarray/begin/
  iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(data_->createIterator(resources_), resources_);
  }

  // Returns an iterator following the last element of the array.
  // https://arduinojson.org/v7/api/jsonarray/end/
  iterator end() const {
    return iterator();
  }

  // Copies an array.
  // https://arduinojson.org/v7/api/jsonarray/set/
  bool set(JsonArrayConst src) const {
    if (!data_)
      return false;

    clear();
    for (auto element : src) {
      if (!add(element))
        return false;
    }

    return true;
  }

  // Removes the element at the specified iterator.
  // https://arduinojson.org/v7/api/jsonarray/remove/
  void remove(iterator it) const {
    detail::ArrayData::remove(data_, it.iterator_, resources_);
  }

  // Removes the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/remove/
  void remove(size_t index) const {
    detail::ArrayData::removeElement(data_, index, resources_);
  }

  // Removes all the elements of the array.
  // https://arduinojson.org/v7/api/jsonarray/clear/
  void clear() const {
    detail::ArrayData::clear(data_, resources_);
  }

  // Gets or sets the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/subscript/
  detail::ElementProxy<JsonArray> operator[](size_t index) const {
    return {*this, index};
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_), resources_);
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonarray/isnull/
  bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonarray/isnull/
  operator bool() const {
    return data_ != 0;
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v7/api/jsonarray/nesting/
  size_t nesting() const {
    return detail::VariantData::nesting(collectionToVariant(data_), resources_);
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v7/api/jsonarray/size/
  size_t size() const {
    return data_ ? data_->size(resources_) : 0;
  }

  // DEPRECATED: use add<JsonVariant>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonVariant>() instead")
  JsonVariant add() const {
    return add<JsonVariant>();
  }

  // DEPRECATED: use add<JsonArray>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonArray>() instead")
  JsonArray createNestedArray() const {
    return add<JsonArray>();
  }

  // DEPRECATED: use add<JsonObject>() instead
  ARDUINOJSON_DEPRECATED("use add<JsonObject>() instead")
  JsonObject createNestedObject() const;

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  detail::ResourceManager* getResourceManager() const {
    return resources_;
  }

  detail::VariantData* getData() const {
    return collectionToVariant(data_);
  }

  detail::VariantData* getOrCreateData() const {
    return collectionToVariant(data_);
  }

  detail::ArrayData* data_;
  detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
