// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/VariantRefBase.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDerived>
inline JsonVariant VariantRefBase<TDerived>::add() const {
  return add<JsonVariant>();
}

template <typename TDerived>
template <typename T>
inline typename enable_if<ConverterNeedsWriteableRef<T>::value, T>::type
VariantRefBase<TDerived>::as() const {
  return Converter<T>::fromJson(getVariant());
}

template <typename TDerived>
inline JsonArray VariantRefBase<TDerived>::createNestedArray() const {
  return add<JsonArray>();
}

template <typename TDerived>
template <typename TChar>
inline JsonArray VariantRefBase<TDerived>::createNestedArray(TChar* key) const {
  return operator[](key).template to<JsonArray>();
}

template <typename TDerived>
template <typename TString>
inline JsonArray VariantRefBase<TDerived>::createNestedArray(
    const TString& key) const {
  return operator[](key).template to<JsonArray>();
}

template <typename TDerived>
inline JsonObject VariantRefBase<TDerived>::createNestedObject() const {
  return add<JsonObject>();
}

template <typename TDerived>
template <typename TChar>
inline JsonObject VariantRefBase<TDerived>::createNestedObject(
    TChar* key) const {
  return operator[](key).template to<JsonObject>();
}

template <typename TDerived>
template <typename TString>
inline JsonObject VariantRefBase<TDerived>::createNestedObject(
    const TString& key) const {
  return operator[](key).template to<JsonObject>();
}

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
template <typename T>
inline typename enable_if<ConverterNeedsWriteableRef<T>::value, bool>::type
VariantRefBase<TDerived>::is() const {
  return Converter<T>::checkJson(getVariant());
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
inline bool VariantRefBase<TDerived>::set(const T& value) const {
  Converter<typename detail::remove_cv<T>::type>::toJson(value,
                                                         getOrCreateVariant());
  auto resources = getResourceManager();
  return resources && !resources->overflowed();
}

template <typename TDerived>
template <typename T>
inline bool VariantRefBase<TDerived>::set(T* value) const {
  Converter<T*>::toJson(value, getOrCreateVariant());
  auto resources = getResourceManager();
  return resources && !resources->overflowed();
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
