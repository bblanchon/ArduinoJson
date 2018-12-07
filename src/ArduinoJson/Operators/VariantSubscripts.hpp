// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringWrappers.hpp"
#include "../Variant/VariantAs.hpp"

namespace ARDUINOJSON_NAMESPACE {
class ArrayRef;
class ObjectRef;

// Forward declarations.
class ArraySubscript;
template <typename TKey>
class ObjectSubscript;

template <typename TImpl>
class VariantSubscripts {
 public:
  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE ArraySubscript operator[](size_t index) const;

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  //
  // ObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  ObjectSubscript<const TString &> >::type
  operator[](const TString &key) const;
  //
  // ObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString *>::value,
                                  ObjectSubscript<TString *> >::type
  operator[](TString *key) const;

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
