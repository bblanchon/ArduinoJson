// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_LITTLE_ENDIAN
inline void swapBytes(uint8_t &a, uint8_t &b) {
  uint8_t t(a);
  a = b;
  b = t;
}

inline void fixEndianess(uint8_t *p, integral_constant<size_t, 8>) {
  swapBytes(p[0], p[7]);
  swapBytes(p[1], p[6]);
  swapBytes(p[2], p[5]);
  swapBytes(p[3], p[4]);
}

inline void fixEndianess(uint8_t *p, integral_constant<size_t, 4>) {
  swapBytes(p[0], p[3]);
  swapBytes(p[1], p[2]);
}

inline void fixEndianess(uint8_t *p, integral_constant<size_t, 2>) {
  swapBytes(p[0], p[1]);
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
