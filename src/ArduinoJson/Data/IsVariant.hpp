// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonVariantTag {};

template <typename T>
struct IsVariant : is_base_of<JsonVariantTag, T> {};
}  // namespace ARDUINOJSON_NAMESPACE
