// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
inline ArrayRef ObjectRef::createNestedArray(const TString& key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TChar>
inline ArrayRef ObjectRef::createNestedArray(TChar* key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TDataSource>
template <typename TString>
inline ArrayRef VariantRefBase<TDataSource>::createNestedArray(
    const TString& key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TDataSource>
template <typename TChar>
inline ArrayRef VariantRefBase<TDataSource>::createNestedArray(
    TChar* key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TDataSource>
template <typename TString>
inline ObjectRef VariantRefBase<TDataSource>::createNestedObject(
    const TString& key) const {
  return operator[](key).template to<ObjectRef>();
}

template <typename TDataSource>
template <typename TChar>
inline ObjectRef VariantRefBase<TDataSource>::createNestedObject(
    TChar* key) const {
  return operator[](key).template to<ObjectRef>();
}

template <typename TDataSource>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
VariantRefBase<TDataSource>::containsKey(const TString& key) const {
  return variantGetMember(VariantAttorney::getData(*this), adaptString(key)) !=
         0;
}

template <typename TDataSource>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
VariantRefBase<TDataSource>::containsKey(TChar* key) const {
  return variantGetMember(VariantAttorney::getData(*this), adaptString(key)) !=
         0;
}

template <typename TDataSource>
template <typename TString>
inline typename enable_if<IsString<TString*>::value,
                          VariantProxy<MemberDataSource<
                              VariantRefBase<TDataSource>, TString*> > >::type
VariantRefBase<TDataSource>::operator[](TString* key) const {
  return VariantProxy<MemberDataSource<VariantRefBase, TString*> >(
      MemberDataSource<VariantRefBase, TString*>(*this, key));
}

template <typename TDataSource>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          VariantProxy<MemberDataSource<
                              VariantRefBase<TDataSource>, TString> > >::type
VariantRefBase<TDataSource>::operator[](const TString& key) const {
  return VariantProxy<MemberDataSource<VariantRefBase, TString> >(
      MemberDataSource<VariantRefBase, TString>(*this, key));
}

}  // namespace ARDUINOJSON_NAMESPACE
