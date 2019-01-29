// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringWrappers.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename TParent, typename TKey>
class MemberProxy;

template <typename TObject>
class ObjectShortcuts {
 public:
  // MemberProxy operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE
      typename enable_if<IsString<TKey>::value,
                         MemberProxy<const TObject &, const TKey &> >::type
      operator[](const TKey &key) const;
  //
  // MemberProxy operator[](TKey) const;
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey *>::value,
                                  MemberProxy<const TObject &, TKey *> >::type
  operator[](TKey *key) const;

  // Creates and adds a ArrayRef.
  //
  // ArrayRef createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(const TKey &key) const;
  // ArrayRef createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE ArrayRef createNestedArray(TKey *key) const;

  // Creates and adds a ObjectRef.
  //
  // ObjectRef createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TKey>
  ObjectRef createNestedObject(const TKey &key) const;
  //
  // ObjectRef createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
  template <typename TKey>
  ObjectRef createNestedObject(TKey *key) const;

 private:
  const TObject *impl() const {
    return static_cast<const TObject *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
