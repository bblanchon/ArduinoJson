// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Document/JsonDocument.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Copies a value to a JsonVariant.
// This is a degenerated form of copyArray() to stop the recursion.
template <typename T>
inline typename detail::enable_if<!detail::is_array<T>::value, bool>::type
copyArray(const T& src, JsonVariant dst) {
  return dst.set(src);
}

// Copies values from an array to a JsonArray or a JsonVariant.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T, size_t N, typename TDestination>
inline typename detail::enable_if<
    !detail::is_base_of<JsonDocument, TDestination>::value, bool>::type
copyArray(T (&src)[N], const TDestination& dst) {
  return copyArray(src, N, dst);
}

// Copies values from an array to a JsonArray or a JsonVariant.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T, typename TDestination>
inline typename detail::enable_if<
    !detail::is_base_of<JsonDocument, TDestination>::value, bool>::type
copyArray(const T* src, size_t len, const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < len; i++) {
    ok &= copyArray(src[i], dst.add());
  }
  return ok;
}

// Copies a string to a JsonVariant.
// This is a degenerated form of copyArray() to handle strings.
template <typename TDestination>
inline bool copyArray(const char* src, size_t, const TDestination& dst) {
  return dst.set(src);
}

// Copies values from an array to a JsonDocument.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T>
inline bool copyArray(const T& src, JsonDocument& dst) {
  return copyArray(src, dst.to<JsonArray>());
}

// Copies an array to a JsonDocument.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T>
inline bool copyArray(const T* src, size_t len, JsonDocument& dst) {
  return copyArray(src, len, dst.to<JsonArray>());
}

// Copies a value from a JsonVariant.
// This is a degenerated form of copyArray() to stop the recursion.
template <typename T>
inline typename detail::enable_if<!detail::is_array<T>::value, size_t>::type
copyArray(JsonVariantConst src, T& dst) {
  dst = src.as<T>();
  return 1;
}

// Copies values from a JsonArray or JsonVariant to an array.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T, size_t N>
inline size_t copyArray(JsonArrayConst src, T (&dst)[N]) {
  return copyArray(src, dst, N);
}

// Copies values from a JsonArray or JsonVariant to an array.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename T>
inline size_t copyArray(JsonArrayConst src, T* dst, size_t len) {
  size_t i = 0;
  for (JsonArrayConst::iterator it = src.begin(); it != src.end() && i < len;
       ++it)
    copyArray(*it, dst[i++]);
  return i;
}

// Copies a string from a JsonVariant.
// This is a degenerated form of copyArray() to handle strings.
template <size_t N>
inline size_t copyArray(JsonVariantConst src, char (&dst)[N]) {
  JsonString s = src;
  size_t len = N - 1;
  if (len > s.size())
    len = s.size();
  memcpy(dst, s.c_str(), len);
  dst[len] = 0;
  return 1;
}

// Copies values from a JsonDocument to an array.
// https://arduinojson.org/v6/api/misc/copyarray/
template <typename TSource, typename T>
inline typename detail::enable_if<
    detail::is_array<T>::value &&
        detail::is_base_of<JsonDocument, TSource>::value,
    size_t>::type
copyArray(const TSource& src, T& dst) {
  return copyArray(src.template as<JsonArrayConst>(), dst);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
