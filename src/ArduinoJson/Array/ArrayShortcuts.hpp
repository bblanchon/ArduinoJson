// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {
// Forward declarations.
class ArrayRef;
class ObjectRef;
template <typename>
class ElementProxy;

template <typename TArray>
class ArrayShortcuts {
 public:
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE ElementProxy<TArray> operator[](size_t index) const;

  FORCE_INLINE ObjectRef createNestedObject() const;

  FORCE_INLINE ArrayRef createNestedArray() const;

  // Adds the specified value at the end of the array.
  //
  // bool add(TValue);
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ObjectRef
  template <typename T>
  FORCE_INLINE bool add(const T &value) const {
    return impl()->addElement().set(value);
  }
  //
  // bool add(TValue);
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename T>
  FORCE_INLINE bool add(T *value) const {
    return impl()->addElement().set(value);
  }

 private:
  const TArray *impl() const {
    return static_cast<const TArray *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
