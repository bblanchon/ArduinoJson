// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Object/ObjectRef.hpp"
#include "ArrayRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TArray>
inline ArrayRef ArrayShortcuts<TArray>::createNestedArray() const {
  return impl()->addElement().template to<ArrayRef>();
}

template <typename TArray>
inline ObjectRef ArrayShortcuts<TArray>::createNestedObject() const {
  return impl()->addElement().template to<ObjectRef>();
}

}  // namespace ARDUINOJSON_NAMESPACE
