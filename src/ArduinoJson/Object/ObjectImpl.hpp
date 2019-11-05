// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    const TString& key) const {
  return impl()->getOrAddMember(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TChar>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(TChar* key) const {
  return impl()->getOrAddMember(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TString>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    const TString& key) const {
  return impl()->getOrAddMember(key).template to<ObjectRef>();
}

template <typename TObject>
template <typename TChar>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    TChar* key) const {
  return impl()->getOrAddMember(key).template to<ObjectRef>();
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(const TString& key) const {
  return !impl()->getMember(key).isUndefined();
}

template <typename TObject>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(TChar* key) const {
  return !impl()->getMember(key).isUndefined();
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString*>::value,
                          MemberProxy<TObject, TString*> >::type
    ObjectShortcuts<TObject>::operator[](TString* key) const {
  return MemberProxy<TObject, TString*>(*impl(), key);
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsWriteableString<TString>::value,
                          MemberProxy<TObject, TString> >::type
    ObjectShortcuts<TObject>::operator[](const TString& key) const {
  return MemberProxy<TObject, TString>(*impl(), key);
}

template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value &&
                              !IsWriteableString<TString>::value,
                          MemberProxy<TObject, const TString&> >::type
    ObjectShortcuts<TObject>::operator[](const TString& key) const {
  return MemberProxy<TObject, const TString&>(*impl(), key);
}

}  // namespace ARDUINOJSON_NAMESPACE
