// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE
class JsonArray;
class JsonObject;
class JsonVariant;
ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE
// A metafunction that returns the type of the value returned by
// JsonVariant::to<T>()
template <typename T>
struct VariantTo {};

template <>
struct VariantTo<JsonArray> {
  using type = JsonArray;
};
template <>
struct VariantTo<JsonObject> {
  using type = JsonObject;
};
template <>
struct VariantTo<JsonVariant> {
  using type = JsonVariant;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
