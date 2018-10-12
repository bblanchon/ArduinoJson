// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"
#include "Polyfills/type_traits.hpp"
#include "Strings/StringTypes.hpp"

namespace ARDUINOJSON_NAMESPACE {
class JsonArray;
class JsonObject;

// Forward declarations.
class JsonArraySubscript;
template <typename TKey>
class JsonObjectSubscript;

template <typename TImpl>
class JsonVariantSubscripts {
 public:
  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE JsonArraySubscript operator[](size_t index) const;

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  //
  // JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key) const;
  //
  // JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString *>::value,
                                  JsonObjectSubscript<TString *> >::type
  operator[](TString *key) const;

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
