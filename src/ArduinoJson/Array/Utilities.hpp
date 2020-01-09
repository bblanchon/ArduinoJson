// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Array/ArrayRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Copy a 1D array to a JsonArray
template <typename T, size_t N>
inline bool copyArray(T (&src)[N], ArrayRef dst) {
  return copyArray(src, N, dst);
}

// Copy a 1D array to a JsonArray
template <typename T>
inline bool copyArray(T* src, size_t len, ArrayRef dst) {
  bool ok = true;
  for (size_t i = 0; i < len; i++) {
    ok &= dst.add(src[i]);
  }
  return ok;
}

// Copy a 2D array to a JsonArray
template <typename T, size_t N1, size_t N2>
inline bool copyArray(T (&src)[N1][N2], ArrayRef dst) {
  bool ok = true;
  for (size_t i = 0; i < N1; i++) {
    ArrayRef nestedArray = dst.createNestedArray();
    for (size_t j = 0; j < N2; j++) {
      ok &= nestedArray.add(src[i][j]);
    }
  }
  return ok;
}

// Copy a JsonArray to a 1D array
template <typename T, size_t N>
inline size_t copyArray(ArrayConstRef src, T (&dst)[N]) {
  return copyArray(src, dst, N);
}

// Copy a JsonArray to a 1D array
template <typename T>
inline size_t copyArray(ArrayConstRef src, T* dst, size_t len) {
  size_t i = 0;
  for (ArrayConstRef::iterator it = src.begin(); it != src.end() && i < len;
       ++it)
    dst[i++] = *it;
  return i;
}

// Copy a JsonArray to a 2D array
template <typename T, size_t N1, size_t N2>
inline void copyArray(ArrayConstRef src, T (&dst)[N1][N2]) {
  size_t i = 0;
  for (ArrayConstRef::iterator it = src.begin(); it != src.end() && i < N1;
       ++it) {
    copyArray(it->as<ArrayConstRef>(), dst[i++]);
  }
}

}  // namespace ARDUINOJSON_NAMESPACE
