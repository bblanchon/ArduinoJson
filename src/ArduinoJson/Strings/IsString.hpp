// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable = void>
struct IsString : false_type {};

template <typename TChar>
struct IsString<const TChar*> : IsString<TChar*> {};

}  // namespace ARDUINOJSON_NAMESPACE
