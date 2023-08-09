// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
inline void convertToJson(const VariantRefBase<TDerived>& src,
                          JsonVariant dst) {
  dst.set(src.template as<JsonVariantConst>());
}

template <typename TDerived>
template <typename T>
inline typename enable_if<is_same<T, JsonVariant>::value, T>::type
VariantRefBase<TDerived>::add() const {
  return JsonVariant(
      detail::VariantData::addElement(getOrCreateData(), getResourceManager()),
      getResourceManager());
}

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
inline JsonVariant VariantRefBase<TDerived>::getVariant() const {
  return JsonVariant(getData(), getResourceManager());
}

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::getOrCreateVariant() const {
  return JsonVariant(getOrCreateData(), getResourceManager());
}

template <typename TDerived>
inline ElementProxy<TDerived> VariantRefBase<TDerived>::operator[](
    size_t index) const {
  return ElementProxy<TDerived>(derived(), index);
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

template <typename TDerived>
template <typename T>
inline typename enable_if<is_same<T, JsonArray>::value, JsonArray>::type
VariantRefBase<TDerived>::to() const {
  return JsonArray(
      VariantData::toArray(getOrCreateData(), getResourceManager()),
      getResourceManager());
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonObject>::value, JsonObject>::type
VariantRefBase<TDerived>::to() const {
  return JsonObject(
      VariantData::toObject(getOrCreateData(), getResourceManager()),
      getResourceManager());
}

template <typename TDerived>
template <typename T>
typename enable_if<is_same<T, JsonVariant>::value, JsonVariant>::type
VariantRefBase<TDerived>::to() const {
  auto data = getOrCreateData();
  auto resources = getResourceManager();
  detail::VariantData::setNull(data, resources);
  return JsonVariant(data, resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
