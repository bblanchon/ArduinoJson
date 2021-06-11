// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable = void>
struct Converter;

// clang-format off
template <typename T1, typename T2>
class InvalidConversion;  // Error here? See https://arduinojson.org/v6/invalid-conversion/
// clang-format on

}  // namespace ARDUINOJSON_NAMESPACE
