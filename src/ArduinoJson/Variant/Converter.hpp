// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T, typename Enable = void>
struct Converter;

ARDUINOJSON_END_PUBLIC_NAMESPACE

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// clang-format off
template <typename T1, typename T2>
class InvalidConversion;  // Error here? See https://arduinojson.org/v7/invalid-conversion/
// clang-format on

ARDUINOJSON_END_PRIVATE_NAMESPACE
