// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/ForceInline.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArraySubscript;
class JsonObjectSubscript;

template <typename TImpl>
class JsonVariantBase {
 public:
  // Gets the variant as a boolean value.
  // Returns false if the variant is not a boolean value.
  JSON_FORCE_INLINE operator bool() const { return as<bool>(); }

  // Gets the variant as a floating-point value.
  // Returns 0.0 if the variant is not a floating-point value
  JSON_FORCE_INLINE operator double() const { return as<double>(); }
  JSON_FORCE_INLINE operator float() const { return as<float>(); }

  // Gets the variant as an integer value.
  // Returns 0 if the variant is not an integer value.
  JSON_FORCE_INLINE operator signed long() const { return as<signed long>(); }
  JSON_FORCE_INLINE operator signed char() const { return as<signed char>(); }
  JSON_FORCE_INLINE operator signed int() const { return as<signed int>(); }
  JSON_FORCE_INLINE operator signed short() const { return as<signed short>(); }
  JSON_FORCE_INLINE operator unsigned char() const {
    return as<unsigned char>();
  }
  JSON_FORCE_INLINE operator unsigned int() const { return as<unsigned int>(); }
  JSON_FORCE_INLINE operator unsigned long() const {
    return as<unsigned long>();
  }
  JSON_FORCE_INLINE operator unsigned short() const {
    return as<unsigned short>();
  }

  // Gets the variant as a string.
  // Returns NULL if variant is not a string.
  JSON_FORCE_INLINE operator const char *() const { return as<const char *>(); }
  JSON_FORCE_INLINE const char *asString() const { return as<const char *>(); }

  // Gets the variant as an array.
  // Returns a reference to the JsonArray or JsonArray::invalid() if the
  // variant
  // is not an array.
  JSON_FORCE_INLINE operator JsonArray &() const { return as<JsonArray &>(); }
  JSON_FORCE_INLINE JsonArray &asArray() const { return as<JsonArray &>(); }

  // Gets the variant as an object.
  // Returns a reference to the JsonObject or JsonObject::invalid() if the
  // variant is not an object.
  JSON_FORCE_INLINE operator JsonObject &() const { return as<JsonObject &>(); }
  JSON_FORCE_INLINE JsonObject &asObject() const { return as<JsonObject &>(); }

  template <typename T>
  JSON_FORCE_INLINE const T as() const {
    return impl()->template as<T>();
  }

  // Mimics an array or an object.
  // Returns the size of the array or object if the variant has that type.
  // Returns 0 if the variant is neither an array nor an object
  size_t size() const { return asArray().size() + asObject().size(); }

  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  // Returns JsonVariant::invalid() if the variant is not an array.
  JSON_FORCE_INLINE const JsonArraySubscript operator[](int index) const;

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  // Return JsonVariant::invalid() if the variant is not an object.
  JSON_FORCE_INLINE const JsonObjectSubscript operator[](const char *key) const;

 private:
  const TImpl *impl() const { return static_cast<const TImpl *>(this); }
};

template <typename TImpl, typename TComparand>
inline bool operator==(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() == right;
}

template <typename TImpl, typename TComparand>
inline bool operator==(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left == right.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator!=(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() != right;
}

template <typename TImpl, typename TComparand>
inline bool operator!=(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left != right.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator<=(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() <= right;
}

template <typename TImpl, typename TComparand>
inline bool operator<=(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left <= right.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator>=(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() >= right;
}

template <typename TImpl, typename TComparand>
inline bool operator>=(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left >= right.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator<(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() < right;
}

template <typename TImpl, typename TComparand>
inline bool operator<(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left < right.template as<TComparand>();
}

template <typename TImpl, typename TComparand>
inline bool operator>(const JsonVariantBase<TImpl> &left, TComparand right) {
  return left.template as<TComparand>() > right;
}

template <typename TImpl, typename TComparand>
inline bool operator>(TComparand left, const JsonVariantBase<TImpl> &right) {
  return left > right.template as<TComparand>();
}
}
