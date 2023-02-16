// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

inline JsonObject JsonArray::createNestedObject() const {
  return add().to<JsonObject>();
}

ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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

ARDUINOJSON_END_PRIVATE_NAMESPACE
