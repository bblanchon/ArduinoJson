// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T, typename Enable = void>
struct IsString : false_type {};

template <typename T>
struct IsString<T, void_t<typename StringAdapterFor<T>::AdaptedString>>
    : true_type {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
