// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    const TString& key) const {
  return impl()->operator[](key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TChar>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(TChar* key) const {
  return impl()->operator[](key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TString>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    const TString& key) const {
  return impl()->operator[](key).template to<ObjectRef>();
}

template <typename TObject>
template <typename TChar>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    TChar* key) const {
  return impl()->operator[](key).template to<ObjectRef>();
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(const TString& key) const {
  return variantGetMember(VariantAttorney::getData(*impl()),
                          adaptString(key)) != 0;
}

template <typename TObject>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(TChar* key) const {
  return variantGetMember(VariantAttorney::getData(*impl()),
                          adaptString(key)) != 0;
}

template <typename TObject>
template <typename TString>
inline typename enable_if<
    IsString<TString*>::value,
    VariantProxy<MemberDataSource<TObject, TString*> > >::type
ObjectShortcuts<TObject>::operator[](TString* key) const {
  return VariantProxy<MemberDataSource<TObject, TString*> >(
      MemberDataSource<TObject, TString*>(*impl(), key));
}

template <typename TObject>
template <typename TString>
inline
    typename enable_if<IsString<TString>::value,
                       VariantProxy<MemberDataSource<TObject, TString> > >::type
    ObjectShortcuts<TObject>::operator[](const TString& key) const {
  return VariantProxy<MemberDataSource<TObject, TString> >(
      MemberDataSource<TObject, TString>(*impl(), key));
}

}  // namespace ARDUINOJSON_NAMESPACE
