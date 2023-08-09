// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
VariantRefBase<TDerived>::containsKey(const TString& key) const {
  return VariantData::getMember(getData(), adaptString(key),
                                getResourceManager()) != 0;
}

template <typename TDerived>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
VariantRefBase<TDerived>::containsKey(TChar* key) const {
  return VariantData::getMember(getData(), adaptString(key),
                                getResourceManager()) != 0;
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
