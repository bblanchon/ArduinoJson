// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

struct VariantTag {};

template <typename T>
struct IsVariant : is_base_of<VariantTag, T> {};

}  // namespace ARDUINOJSON_NAMESPACE
