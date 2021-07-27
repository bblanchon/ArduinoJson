// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, bool bounded = false, typename Enable = void>
class StringAdapter;

template <typename T>
inline StringAdapter<T, false> adaptString(const T& str) {
  return StringAdapter<T, false>(str);
}

template <typename T>
inline StringAdapter<T, true> adaptString(const T& str, size_t sz) {
  return StringAdapter<T, true>(str, sz);
}

template <typename T, typename Enable = void>
struct IsString : false_type {};

template <typename T>
struct IsString<
    T, typename make_void<typename StringAdapter<T>::storage_policy>::type>
    : true_type {};

}  // namespace ARDUINOJSON_NAMESPACE
