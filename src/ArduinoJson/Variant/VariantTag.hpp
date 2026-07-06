// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct VariantTag {};

template <typename T>
struct IsVariant : is_base_of<VariantTag, T> {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
