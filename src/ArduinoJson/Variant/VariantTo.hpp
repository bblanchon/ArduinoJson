// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {
class JsonArray;
class JsonObject;
class VariantRef;

// A metafunction that returns the type of the value returned by
// VariantRef::to<T>()
template <typename T>
struct VariantTo {};

template <>
struct VariantTo<JsonArray> {
  typedef JsonArray type;
};
template <>
struct VariantTo<JsonObject> {
  typedef JsonObject type;
};
template <>
struct VariantTo<VariantRef> {
  typedef VariantRef type;
};

}  // namespace ARDUINOJSON_NAMESPACE
