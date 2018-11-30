// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Array/ArrayRef.hpp"
#include "ObjectRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
inline ArrayRef ObjectRef::createNestedArray(const TString& key) const {
  return set(key).template to<ArrayRef>();
}

template <typename TString>
inline ArrayRef ObjectRef::createNestedArray(TString* key) const {
  return set(key).template to<ArrayRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
