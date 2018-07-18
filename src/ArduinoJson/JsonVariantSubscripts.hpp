// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"
#include "Polyfills/type_traits.hpp"
#include "Strings/StringTypes.hpp"

namespace ArduinoJson {
class JsonArray;
class JsonObject;
namespace Internals {

// Forward declarations.
class JsonArraySubscript;
template <typename TKey>
class JsonObjectSubscript;

template <typename TImpl>
class JsonVariantSubscripts {
 public:
  // Mimics an array or an object.
  // Returns the size of the array or object if the variant has that type.
  // Returns 0 if the variant is neither an array nor an object
  size_t size() const {
    return impl()->template as<JsonArray>().size() +
           impl()->template as<JsonObject>().size();
  }

  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE const JsonArraySubscript operator[](size_t index) const;
  FORCE_INLINE JsonArraySubscript operator[](size_t index);

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value,
                         const JsonObjectSubscript<const TString &> >::type
      operator[](const TString &key) const;
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key);
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString *>::value,
                                  JsonObjectSubscript<TString *> >::type
  operator[](TString *key);
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString *>::value,
                                  const JsonObjectSubscript<TString *> >::type
  operator[](TString *key) const;

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
