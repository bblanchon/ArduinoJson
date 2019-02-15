// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringAdapters.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename TParent, typename TStringRef>
class MemberProxy;

template <typename TObject>
class ObjectShortcuts {
 public:
  // operator[](const std::string&) const
  // operator[](const String&) const
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value,
                         MemberProxy<const TObject &, const TString &> >::type
      operator[](const TString &key) const;

  // operator[](char*) const
  // operator[](const char*) const
  // operator[](const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value,
                                  MemberProxy<const TObject &, TChar *> >::type
  operator[](TChar *key) const;

  // createNestedArray(const std::string&) const
  // createNestedArray(const String&) const
  template <typename TString>
  FORCE_INLINE ArrayRef createNestedArray(const TString &key) const;

  // createNestedArray(char*) const
  // createNestedArray(const char*) const
  // createNestedArray(const __FlashStringHelper*) const
  template <typename TChar>
  FORCE_INLINE ArrayRef createNestedArray(TChar *key) const;

  // createNestedObject(const std::string&) const
  // createNestedObject(const String&) const
  template <typename TString>
  ObjectRef createNestedObject(const TString &key) const;

  // createNestedObject(char*) const
  // createNestedObject(const char*) const
  // createNestedObject(const __FlashStringHelper*) const
  template <typename TChar>
  ObjectRef createNestedObject(TChar *key) const;

 private:
  const TObject *impl() const {
    return static_cast<const TObject *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
