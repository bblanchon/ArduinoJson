// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
  using iterator = JsonArrayIterator;

  // Constructs an unbound reference.
  JsonArray() {}

  // INTERNAL USE ONLY
  JsonArray(detail::VariantData* data, detail::ResourceManager* resources)
      : impl_(data, resources) {}

  // INTERNAL USE ONLY
  JsonArray(const detail::VariantImpl& impl) : impl_(impl) {}

  // Returns a JsonVariant pointing to the array.
  // https://arduinojson.org/v7/api/jsonvariant/
  operator JsonVariant() {
    return JsonVariant(getData(), getResourceManager());
  }

  // Returns a read-only reference to the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/
  operator JsonArrayConst() const {
    return JsonArrayConst(getData(), getResourceManager());
  }

  // Appends a new (empty) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T, detail::enable_if_t<
                            !detail::is_same<T, JsonVariant>::value, int> = 0>
  T add() const {
    return add<JsonVariant>().to<T>();
  }

  // Appends a new (null) element to the array.
  // Returns a reference to the new element.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T, detail::enable_if_t<
                            detail::is_same<T, JsonVariant>::value, int> = 0>
  JsonVariant add() const {
    return JsonVariant(impl_.addNewElement(), impl_.resources());
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T>
  bool add(const T& value) const {
    if (!impl_.isArray())
      return false;
    return addValue(value, impl_.data(), impl_.resources());
  }

  // Appends a value to the array.
  // https://arduinojson.org/v7/api/jsonarray/add/
  template <typename T,
            detail::enable_if_t<!detail::is_const<T>::value, int> = 0>
  bool add(T* value) const {
    if (!impl_.isArray())
      return false;
    return addValue(value, impl_.data(), impl_.resources());
  }

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v7/api/jsonarray/begin/
  iterator begin() const {
    return iterator(impl_.createIterator(), impl_.resources());
  }

  // Returns an iterator following the last element of the array.
  // https://arduinojson.org/v7/api/jsonarray/end/
  iterator end() const {
    return iterator();
  }

  // Copies an array.
  // https://arduinojson.org/v7/api/jsonarray/set/
  bool set(JsonArrayConst src) const {
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
    impl_.removeElement(it.iterator_);
  }

  // Removes the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/remove/
  void remove(size_t index) const {
    impl_.removeElement(index);
  }

  // Removes the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/remove/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  void remove(const TVariant& variant) const {
    if (variant.template is<size_t>())
      remove(variant.template as<size_t>());
  }

  // Removes all the elements of the array.
  // https://arduinojson.org/v7/api/jsonarray/clear/
  void clear() const {
    impl_.empty();
  }

  // Gets or sets the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/subscript/
  template <typename T,
            detail::enable_if_t<detail::is_integral<T>::value, int> = 0>
  detail::ElementProxy<JsonArray> operator[](T index) const {
    return {*this, size_t(index)};
  }

  // Gets or sets the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarray/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  detail::ElementProxy<JsonArray> operator[](const TVariant& variant) const {
    if (variant.template is<size_t>())
      return {*this, variant.template as<size_t>()};
    else
      return {*this, size_t(-1)};
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(getData(), getResourceManager());
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonarray/isnull/
  bool isNull() const {
    return impl_.isNull();
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonarray/isnull/
  operator bool() const {
    return !isNull();
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v7/api/jsonarray/nesting/
  size_t nesting() const {
    return impl_.nesting();
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v7/api/jsonarray/size/
  size_t size() const {
    return impl_.size();
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
    return impl_.resources();
  }

  detail::VariantData* getData() const {
    return impl_.data();
  }

  detail::VariantData* getOrCreateData() const {
    return impl_.data();
  }

  // HACK: this function has been pulled out of VariantImpl to avoid the
  // circular dependency between VariantImpl and JsonVariant
  template <typename T>
  static bool addValue(const T& value, detail::VariantData* data,
                       detail::ResourceManager* resources) {
    ARDUINOJSON_ASSERT(data != nullptr);
    ARDUINOJSON_ASSERT(data->isArray());
    ARDUINOJSON_ASSERT(resources != nullptr);

    auto slot = resources->allocVariant();
    if (!slot)
      return false;

    if (!JsonVariant(slot.ptr(), resources).set(value)) {
      detail::VariantImpl::freeVariant(slot, resources);
      return false;
    }

    detail::VariantImpl::addElement(slot, data, resources);
    return true;
  }

  mutable detail::VariantImpl impl_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
