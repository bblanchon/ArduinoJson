// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename T>
struct is_array : false_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T, size_t N>
struct is_array<T[N]> : true_type {};
}  // namespace Internals
}  // namespace ArduinoJson
