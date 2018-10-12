// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
inline JsonArray JsonObject::createNestedArray(const TString& key) const {
  return set(key).template to<JsonArray>();
}

template <typename TString>
inline JsonArray JsonObject::createNestedArray(TString* key) const {
  return set(key).template to<JsonArray>();
}
}  // namespace ARDUINOJSON_NAMESPACE
