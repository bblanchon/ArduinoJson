// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Array/ArrayRef.hpp"
#include "ObjectRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    const TString& key) const {
  return impl()->getOrCreate(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    TString* key) const {
  return impl()->getOrCreate(key).template to<ArrayRef>();
}

template <typename TObject>
template <typename TKey>
ObjectRef ObjectShortcuts<TObject>::createNestedObject(const TKey& key) const {
  return impl()->getOrCreate(key).template to<ObjectRef>();
}
//
// ObjectRef createNestedObject(TKey);
// TKey = char*, const char*, char[], const char[], const __FlashStringHelper*
template <typename TObject>
template <typename TKey>
ObjectRef ObjectShortcuts<TObject>::createNestedObject(TKey* key) const {
  return impl()->getOrCreate(key).template to<ObjectRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
