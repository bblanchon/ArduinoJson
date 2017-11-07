// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

#include "IsBaseOf.hpp"

namespace ArduinoJson {
namespace TypeTraits {

class JsonVariantTag {};

template <typename T>
struct IsVariant : IsBaseOf<JsonVariantTag, T> {};
}
}
