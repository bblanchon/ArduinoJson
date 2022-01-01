// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>
#include <ArduinoJson/Document/JsonDocument.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Trivial form to stop the recursion
template <typename T>
inline typename enable_if<!is_array<T>::value, bool>::type copyArray(
    const T& src, VariantRef dst) {
  return dst.set(src);
}

// Copy array to a JsonArray/JsonVariant/MemberProxy/ElementProxy
template <typename T, size_t N, typename TDestination>
inline typename enable_if<!is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T (&src)[N], const TDestination& dst) {
  return copyArray(src, N, dst);
}

// Copy ptr+size to a JsonArray/JsonVariant/MemberProxy/ElementProxy
template <typename T, typename TDestination>
inline typename enable_if<!is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(const T* src, size_t len, const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < len; i++) {
    ok &= copyArray(src[i], dst.addElement());
  }
  return ok;
}

// Special case for char[] which much be treated as const char*
template <typename TDestination>
inline bool copyArray(const char* src, size_t, const TDestination& dst) {
  return dst.set(src);
}

// Copy array to a JsonDocument
template <typename T>
inline bool copyArray(const T& src, JsonDocument& dst) {
  return copyArray(src, dst.to<ArrayRef>());
}

// Copy a ptr+size array to a JsonDocument
template <typename T>
inline bool copyArray(const T* src, size_t len, JsonDocument& dst) {
  return copyArray(src, len, dst.to<ArrayRef>());
}

// Trivial case form to stop the recursion
template <typename T>
inline typename enable_if<!is_array<T>::value, size_t>::type copyArray(
    VariantConstRef src, T& dst) {
  dst = src.as<T>();
  return 1;
}

// Copy a JsonArray to array
template <typename T, size_t N>
inline size_t copyArray(ArrayConstRef src, T (&dst)[N]) {
  return copyArray(src, dst, N);
}

// Copy a JsonArray to ptr+size
template <typename T>
inline size_t copyArray(ArrayConstRef src, T* dst, size_t len) {
  size_t i = 0;
  for (ArrayConstRef::iterator it = src.begin(); it != src.end() && i < len;
       ++it)
    copyArray(*it, dst[i++]);
  return i;
}

// Special case for char[] which must be treated as a string
template <size_t N>
inline size_t copyArray(VariantConstRef src, char (&dst)[N]) {
  String s = src;
  size_t len = N - 1;
  if (len > s.size())
    len = s.size();
  memcpy(dst, s.c_str(), len);
  dst[len] = 0;
  return 1;
}

// Copy a JsonDocument to an array
// (JsonDocument doesn't implicitly convert to JsonArrayConst)
template <typename TSource, typename T>
inline typename enable_if<is_array<T>::value &&
                              is_base_of<JsonDocument, TSource>::value,
                          size_t>::type
copyArray(const TSource& src, T& dst) {
  return copyArray(src.template as<ArrayConstRef>(), dst);
}

}  // namespace ARDUINOJSON_NAMESPACE
