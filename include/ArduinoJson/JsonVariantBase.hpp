// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"
#include "Serialization/JsonPrintable.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArraySubscript;
template <typename TKey>
class JsonObjectSubscript;

template <typename TImpl>
class JsonVariantBase : public Internals::JsonPrintable<TImpl> {
 public:
  // DEPRECATED: use as<char*>() instead
  FORCE_INLINE const char *asString() const {
    return as<const char *>();
  }

  // Gets the variant as an array.
  // Returns a reference to the JsonArray or JsonArray::invalid() if the
  // variant
  // is not an array.
  FORCE_INLINE operator JsonArray &() const {
    return as<JsonArray &>();
  }

  // DEPRECATED: use as<JsonArray>() instead
  FORCE_INLINE JsonArray &asArray() const {
    return as<JsonArray &>();
  }

  // Gets the variant as an object.
  // Returns a reference to the JsonObject or JsonObject::invalid() if the
  // variant is not an object.
  FORCE_INLINE operator JsonObject &() const {
    return as<JsonObject &>();
  }

  // DEPRECATED: use as<JsonObject>() instead
  FORCE_INLINE JsonObject &asObject() const {
    return as<JsonObject &>();
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return as<T>();
  }

  template <typename T>
  FORCE_INLINE const typename Internals::JsonVariantAs<T>::type as() const {
    return impl()->template as<T>();
  }

  // Mimics an array or an object.
  // Returns the size of the array or object if the variant has that type.
  // Returns 0 if the variant is neither an array nor an object
  size_t size() const {
    return asArray().size() + asObject().size();
  }

  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  // Returns JsonVariant::invalid() if the variant is not an array.
  FORCE_INLINE const JsonArraySubscript operator[](int index) const;
  FORCE_INLINE JsonArraySubscript operator[](int index);

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  // Return JsonVariant::invalid() if the variant is not an object.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString>::has_equals,
      const JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key) const {
    return asObject()[key];
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString>::has_equals,
      JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key) {
    return asObject()[key];
  }
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString *>::has_equals,
      JsonObjectSubscript<const TString *> >::type
  operator[](const TString *key) {
    return asObject()[key];
  }
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString *>::has_equals,
      const JsonObjectSubscript<const TString *> >::type
  operator[](const TString *key) const {
    return asObject()[key];
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}
