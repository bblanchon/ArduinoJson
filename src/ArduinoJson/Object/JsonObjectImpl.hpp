// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename TString>
inline JsonArray JsonObject::createNestedArray(const TString& key) const {
  return operator[](key).template to<JsonArray>();
}

template <typename TChar>
inline JsonArray JsonObject::createNestedArray(TChar* key) const {
  return operator[](key).template to<JsonArray>();
}

ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
template <typename TString>
inline JsonArray VariantRefBase<TDerived>::createNestedArray(
    const TString& key) const {
  return operator[](key).template to<JsonArray>();
}

template <typename TDerived>
template <typename TChar>
inline JsonArray VariantRefBase<TDerived>::createNestedArray(TChar* key) const {
  return operator[](key).template to<JsonArray>();
}

template <typename TDerived>
template <typename TString>
inline JsonObject VariantRefBase<TDerived>::createNestedObject(
    const TString& key) const {
  return operator[](key).template to<JsonObject>();
}

template <typename TDerived>
template <typename TChar>
inline JsonObject VariantRefBase<TDerived>::createNestedObject(
    TChar* key) const {
  return operator[](key).template to<JsonObject>();
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
                          MemberProxy<TDerived, TString*>>::type
VariantRefBase<TDerived>::operator[](TString* key) const {
  return MemberProxy<TDerived, TString*>(derived(), key);
}

template <typename TDerived>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          MemberProxy<TDerived, TString>>::type
VariantRefBase<TDerived>::operator[](const TString& key) const {
  return MemberProxy<TDerived, TString>(derived(), key);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
