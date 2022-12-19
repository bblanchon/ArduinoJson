// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>

namespace ARDUINOJSON_NAMESPACE {

inline JsonObject JsonArray::createNestedObject() const {
  return add().to<JsonObject>();
}

template <typename TDerived>
inline JsonArray VariantRefBase<TDerived>::createNestedArray() const {
  return add().template to<JsonArray>();
}

template <typename TDerived>
inline JsonObject VariantRefBase<TDerived>::createNestedObject() const {
  return add().template to<JsonObject>();
}

template <typename TDerived>
inline ElementProxy<TDerived> VariantRefBase<TDerived>::operator[](
    size_t index) const {
  return ElementProxy<TDerived>(derived(), index);
}

}  // namespace ARDUINOJSON_NAMESPACE
