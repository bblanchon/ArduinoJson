// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
inline JsonArray JsonObject::createNestedArray(const TString& key) {
  return createNestedArray_impl(makeString(key));
}

template <typename TString>
inline JsonArray JsonObject::createNestedArray(TString* key) {
  return createNestedArray_impl(makeString(key));
}

template <typename TStringRef>
inline JsonArray JsonObject::createNestedArray_impl(TStringRef key) {
  return set_impl(key).template to<JsonArray>();
}

template <typename TStringRef>
inline JsonObject JsonObject::createNestedObject_impl(TStringRef key) {
  return set_impl(key).template to<JsonObject>();
}
}  // namespace ARDUINOJSON_NAMESPACE
