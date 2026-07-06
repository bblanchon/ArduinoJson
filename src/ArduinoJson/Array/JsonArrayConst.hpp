// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
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
  using iterator = JsonArrayConstIterator;

  // Returns an iterator to the first element of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/begin/
  iterator begin() const {
    return iterator(impl_.createIterator(), impl_.resources());
  }

  // Returns an iterator to the element following the last element of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/end/
  iterator end() const {
    return iterator();
  }

  // Creates an unbound reference.
  JsonArrayConst() {}

  // INTERNAL USE ONLY
  JsonArrayConst(detail::VariantData* data, detail::ResourceManager* resources)
      : impl_(data, resources) {}

  // INTERNAL USE ONLY
  JsonArrayConst(const detail::VariantImpl& impl) : impl_(impl) {}

  // Returns the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarrayconst/subscript/
  template <typename T,
            detail::enable_if_t<detail::is_integral<T>::value, int> = 0>
  JsonVariantConst operator[](T index) const {
    return JsonVariantConst(impl_.getElement(size_t(index)), impl_.resources());
  }

  // Returns the element at the specified index.
  // https://arduinojson.org/v7/api/jsonarrayconst/subscript/
  template <typename TVariant,
            detail::enable_if_t<detail::IsVariant<TVariant>::value, int> = 0>
  JsonVariantConst operator[](const TVariant& variant) const {
    if (variant.template is<size_t>())
      return operator[](variant.template as<size_t>());
    else
      return JsonVariantConst();
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(impl_.data(), impl_.resources());
  }

  // Returns true if the reference is unbound.
  // https://arduinojson.org/v7/api/jsonarrayconst/isnull/
  bool isNull() const {
    return impl_.isNull();
  }

  // Returns true if the reference is bound.
  // https://arduinojson.org/v7/api/jsonarrayconst/isnull/
  operator bool() const {
    return !isNull();
  }

  // Returns the depth (nesting level) of the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/nesting/
  size_t nesting() const {
    return impl_.nesting();
  }

  // Returns the number of elements in the array.
  // https://arduinojson.org/v7/api/jsonarrayconst/size/
  size_t size() const {
    return impl_.size();
  }

  // DEPRECATED: always returns zero
  ARDUINOJSON_DEPRECATED("always returns zero")
  size_t memoryUsage() const {
    return 0;
  }

 private:
  const detail::VariantData* getData() const {
    return impl_.data();
  }

  detail::VariantImpl impl_;
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
