// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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

ARDUINOJSON_END_PRIVATE_NAMESPACE
