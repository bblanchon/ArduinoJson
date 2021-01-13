// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Polyfills/utility.hpp>

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_LITTLE_ENDIAN
inline void fixEndianess(uint8_t *p, integral_constant<size_t, 8>) {
  swap(p[0], p[7]);
  swap(p[1], p[6]);
  swap(p[2], p[5]);
  swap(p[3], p[4]);
}

inline void fixEndianess(uint8_t *p, integral_constant<size_t, 4>) {
  swap(p[0], p[3]);
  swap(p[1], p[2]);
}

inline void fixEndianess(uint8_t *p, integral_constant<size_t, 2>) {
  swap(p[0], p[1]);
}

inline void fixEndianess(uint8_t *, integral_constant<size_t, 1>) {}

template <typename T>
inline void fixEndianess(T &value) {
  fixEndianess(reinterpret_cast<uint8_t *>(&value),
               integral_constant<size_t, sizeof(T)>());
}
#else
template <typename T>
inline void fixEndianess(T &) {}
#endif

}  // namespace ARDUINOJSON_NAMESPACE
