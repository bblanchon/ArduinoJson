// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TArray>
inline ArrayRef ArrayShortcuts<TArray>::createNestedArray() const {
  return impl()->addElement().template to<ArrayRef>();
}

template <typename TArray>
inline ObjectRef ArrayShortcuts<TArray>::createNestedObject() const {
  return impl()->addElement().template to<ObjectRef>();
}

template <typename TArray>
inline ElementProxy<TArray> ArrayShortcuts<TArray>::operator[](
    size_t index) const {
  return ElementProxy<TArray>(*impl(), index);
}

}  // namespace ARDUINOJSON_NAMESPACE
