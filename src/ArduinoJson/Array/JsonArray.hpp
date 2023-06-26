// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Array/JsonArrayConst.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonObject;

// A reference to an array in a JsonDocument
// https://arduinojson.org/v6/api/jsonarray/
class JsonArray : public detail::VariantOperators<JsonArray> {
  friend class detail::VariantAttorney;

 public:
  typedef JsonArrayIterator iterator;

  // Constructs an unbound reference.
  FORCE_INLINE JsonArray() : data_(0), resources_(0) {}

  // INTERNAL USE ONLY
  FORCE_INLINE JsonArray(detail::ArrayData* data,
                         detail::ResourceManager* resources)
      : data_(data), resources_(resources) {}

  // Returns a JsonVariant pointing to the array.
  // https://arduinojson.org/v6/api/jsonvariant/
  operator JsonVariant() {
    void* data = data_;  // prevent warning cast-align
    return JsonVariant(reinterpret_cast<detail::VariantData*>(data),
                       resources_);
  }

  // Returns a read-only reference to the array.
  // https://arduinojson.org/v6/api/jsonarrayconst/
  operator JsonArrayConst() const {
    return JsonArrayConst(data_);
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v6/api/jsonarray/add/
  JsonVariant add() const {
    return JsonVariant(detail::ArrayData::addElement(data_, resources_),
                       resources_);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v6/api/jsonarray/add/
  template <typename T>
  FORCE_INLINE bool add(const T& value) const {
    return add().set(value);
  }

  // Appends a value to the array.
  // https://arduinojson.org/v6/api/jsonarray/add/
  template <typename T>
  FORCE_INLINE bool add(T* value) const {
    return add().set(value);
  }

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v6/api/jsonarray/begin/
  FORCE_INLINE iterator begin() const {
    if (!data_)
      return iterator();
    return iterator(resources_, data_->head());
  }

  // Returns an iterator following the last element of the array.
  // https://arduinojson.org/v6/api/jsonarray/end/
  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Copies an array.
  // https://arduinojson.org/v6/api/jsonarray/set/
  FORCE_INLINE bool set(JsonArrayConst src) const {
    return detail::ArrayData::copy(data_, src.data_, resources_);
  }

  // Compares the content of two arrays.
  FORCE_INLINE bool operator==(JsonArray rhs) const {
    return detail::ArrayData::equals(data_, rhs.data_);
  }

  // Removes the element at the specified iterator.
  // ⚠️ Doesn't release the memory associated with the removed element.
  // https://arduinojson.org/v6/api/jsonarray/remove/
  FORCE_INLINE void remove(iterator it) const {
    detail::ArrayData::removeSlot(data_, it.slot_, resources_);
  }

  // Removes the element at the specified index.
  // ⚠️ Doesn't release the memory associated with the removed element.
  // https://arduinojson.org/v6/api/jsonarray/remove/
  FORCE_INLINE void remove(size_t index) const {
    detail::ArrayData::removeElement(data_, index, resources_);
  }

  // Removes all the elements of the array.
  // ⚠️ Doesn't release the memory associated with the removed elements.
  // https://arduinojson.org/v6/api/jsonarray/clear/
  void clear() const {
    detail::ArrayData::clear(data_, resources_);
  }

  // Gets or sets the element at the specified index.
  // https://arduinojson.org/v6/api/jsonarray/subscript/
  FORCE_INLINE detail::ElementProxy<JsonArray> operator[](size_t index) const {
    return {*this, index};
  }

  // Creates an object and appends it to the array.
  // https://arduinojson.org/v6/api/jsonarray/createnestedobject/
  FORCE_INLINE JsonObject createNestedObject() const;

  // Creates an array and appends it to the array.
  // https://arduinojson.org/v6/api/jsonarray/createnestedarray/
  FORCE_INLINE JsonArray createNestedArray() const {
    return add().to<JsonArray>();
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(data_));
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v6/api/jsonarray/isnull/
  FORCE_INLINE bool isNull() const {
    return data_ == 0;
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v6/api/jsonarray/isnull/
  FORCE_INLINE operator bool() const {
    return data_ != 0;
  }

  // Returns the number of bytes occupied by the array.
  // https://arduinojson.org/v6/api/jsonarray/memoryusage/
  FORCE_INLINE size_t memoryUsage() const {
    return data_ ? data_->memoryUsage() : 0;
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v6/api/jsonarray/nesting/
  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(data_));
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v6/api/jsonarray/size/
  FORCE_INLINE size_t size() const {
    return data_ ? data_->size() : 0;
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

template <>
struct Converter<JsonArray> : private detail::VariantAttorney {
  static void toJson(JsonVariantConst src, JsonVariant dst) {
    variantCopyFrom(getData(dst), getData(src), getResourceManager(dst));
  }

  static JsonArray fromJson(JsonVariant src) {
    auto data = getData(src);
    auto resources = getResourceManager(src);
    return JsonArray(data != 0 ? data->asArray() : 0, resources);
  }

  static detail::InvalidConversion<JsonVariantConst, JsonArray> fromJson(
      JsonVariantConst);

  static bool checkJson(JsonVariantConst) {
    return false;
  }

  static bool checkJson(JsonVariant src) {
    auto data = getData(src);
    return data && data->isArray();
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
template <typename T>
inline typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type
VariantRefBase<TDerived>::to() const {
  return JsonArray(variantToArray(getOrCreateData(), getResourceManager()),
                   getResourceManager());
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
