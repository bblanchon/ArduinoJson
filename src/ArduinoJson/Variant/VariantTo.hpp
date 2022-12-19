// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {
class JsonArray;
class JsonObject;
class JsonVariant;

// A metafunction that returns the type of the value returned by
// JsonVariant::to<T>()
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
struct VariantTo<JsonVariant> {
  typedef JsonVariant type;
};

}  // namespace ARDUINOJSON_NAMESPACE
