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

template <typename TDerived>
template <typename TString>
inline ArrayRef VariantRefBase<TDerived>::createNestedArray(
    const TString& key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TDerived>
template <typename TChar>
inline ArrayRef VariantRefBase<TDerived>::createNestedArray(TChar* key) const {
  return operator[](key).template to<ArrayRef>();
}

template <typename TDerived>
template <typename TString>
inline ObjectRef VariantRefBase<TDerived>::createNestedObject(
    const TString& key) const {
  return operator[](key).template to<ObjectRef>();
}

template <typename TDerived>
template <typename TChar>
inline ObjectRef VariantRefBase<TDerived>::createNestedObject(
    TChar* key) const {
  return operator[](key).template to<ObjectRef>();
}

template <typename TDerived>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
VariantRefBase<TDerived>::containsKey(const TString& key) const {
  return variantGetMember(VariantAttorney::getData(derived()),
                          adaptString(key)) != 0;
}

template <typename TDerived>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
VariantRefBase<TDerived>::containsKey(TChar* key) const {
  return variantGetMember(VariantAttorney::getData(derived()),
                          adaptString(key)) != 0;
}

template <typename TDerived>
template <typename TString>
inline typename enable_if<IsString<TString*>::value,
                          MemberProxy<TDerived, TString*> >::type
VariantRefBase<TDerived>::operator[](TString* key) const {
  return MemberProxy<TDerived, TString*>(derived(), key);
}

template <typename TDerived>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          MemberProxy<TDerived, TString> >::type
VariantRefBase<TDerived>::operator[](const TString& key) const {
  return MemberProxy<TDerived, TString>(derived(), key);
}

}  // namespace ARDUINOJSON_NAMESPACE
