// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stddef.h>  // size_t

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
struct is_array : false_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T, size_t N>
struct is_array<T[N]> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
