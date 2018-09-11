// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ArduinoJson {

template <typename TString>
inline JsonArray JsonObject::createNestedArray(const TString& key) {
  return createNestedArray_impl<const TString&>(key);
}

template <typename TString>
inline JsonArray JsonObject::createNestedArray(TString* key) {
  return createNestedArray_impl<TString*>(key);
}

template <typename TStringRef>
inline JsonArray JsonObject::createNestedArray_impl(TStringRef key) {
  if (!_data) return JsonArray();
  return set(key).template to<JsonArray>();
}

template <typename TStringRef>
inline JsonObject JsonObject::createNestedObject_impl(TStringRef key) {
  if (!_data) return JsonObject();
  return set(key).template to<JsonObject>();
}
}  // namespace ArduinoJson
