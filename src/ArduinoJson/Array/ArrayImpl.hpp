// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Object/ObjectRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline ObjectRef ArrayRef::createNestedObject() const {
  return add().to<ObjectRef>();
}

template <typename TDerived>
inline ArrayRef VariantRefBase<TDerived>::createNestedArray() const {
  return add().template to<ArrayRef>();
}

template <typename TDerived>
inline ObjectRef VariantRefBase<TDerived>::createNestedObject() const {
  return add().template to<ObjectRef>();
}

template <typename TDerived>
inline ElementProxy<TDerived> VariantRefBase<TDerived>::operator[](
    size_t index) const {
  return ElementProxy<TDerived>(derived(), index);
}

}  // namespace ARDUINOJSON_NAMESPACE
