// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#ifdef ARDUINO
#  include <Arduino.h>
#else
// Allow using PROGMEM outside of Arduino (issue #1903)
class __FlashStringHelper;
#  include <avr/pgmspace.h>
#endif

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE
// Wraps a const char* so that the our functions are picked only if the
// originals are missing
struct pgm_p {
  pgm_p(const void* p) : address(reinterpret_cast<const char*>(p)) {}
  const char* address;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

#ifndef strlen_P
inline size_t strlen_P(ArduinoJson::detail::pgm_p s) {
  const char* p = s.address;
  ARDUINOJSON_ASSERT(p != NULL);
  while (pgm_read_byte(p))
    p++;
  return size_t(p - s.address);
}
#endif

#ifndef strncmp_P
inline int strncmp_P(const char* a, ArduinoJson::detail::pgm_p b, size_t n) {
  const char* s1 = a;
  const char* s2 = b.address;
  ARDUINOJSON_ASSERT(s1 != NULL);
  ARDUINOJSON_ASSERT(s2 != NULL);
  while (n-- > 0) {
    char c1 = *s1++;
    char c2 = static_cast<char>(pgm_read_byte(s2++));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == 0 /* and c2 as well */)
      return 0;
  }
  return 0;
}
#endif

#ifndef strcmp_P
inline int strcmp_P(const char* a, ArduinoJson::detail::pgm_p b) {
  const char* s1 = a;
  const char* s2 = b.address;
  ARDUINOJSON_ASSERT(s1 != NULL);
  ARDUINOJSON_ASSERT(s2 != NULL);
  for (;;) {
    char c1 = *s1++;
    char c2 = static_cast<char>(pgm_read_byte(s2++));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == 0 /* and c2 as well */)
      return 0;
  }
}
#endif

#ifndef memcmp_P
inline int memcmp_P(const void* a, ArduinoJson::detail::pgm_p b, size_t n) {
  const uint8_t* p1 = reinterpret_cast<const uint8_t*>(a);
  const char* p2 = b.address;
  ARDUINOJSON_ASSERT(p1 != NULL);
  ARDUINOJSON_ASSERT(p2 != NULL);
  while (n-- > 0) {
    uint8_t v1 = *p1++;
    uint8_t v2 = pgm_read_byte(p2++);
    if (v1 != v2)
      return v1 - v2;
  }
  return 0;
}
#endif

#ifndef memcpy_P
inline void* memcpy_P(void* dst, ArduinoJson::detail::pgm_p src, size_t n) {
  uint8_t* d = reinterpret_cast<uint8_t*>(dst);
  const char* s = src.address;
  ARDUINOJSON_ASSERT(d != NULL);
  ARDUINOJSON_ASSERT(s != NULL);
  while (n-- > 0) {
    *d++ = pgm_read_byte(s++);
  }
  return dst;
}
#endif

#ifndef pgm_read_dword
inline uint32_t pgm_read_dword(ArduinoJson::detail::pgm_p p) {
  uint32_t result;
  memcpy_P(&result, p.address, 4);
  return result;
}
#endif

#ifndef pgm_read_float
inline float pgm_read_float(ArduinoJson::detail::pgm_p p) {
  float result;
  memcpy_P(&result, p.address, sizeof(float));
  return result;
}
#endif

#ifndef pgm_read_double
#  if defined(__SIZEOF_DOUBLE__) && defined(__SIZEOF_FLOAT__) && \
      __SIZEOF_DOUBLE__ == __SIZEOF_FLOAT__
inline double pgm_read_double(ArduinoJson::detail::pgm_p p) {
  return pgm_read_float(p.address);
}
#  else
inline double pgm_read_double(ArduinoJson::detail::pgm_p p) {
  double result;
  memcpy_P(&result, p.address, sizeof(double));
  return result;
}
#  endif
#endif

#ifndef pgm_read_ptr
inline void* pgm_read_ptr(ArduinoJson::detail::pgm_p p) {
  void* result;
  memcpy_P(&result, p.address, sizeof(result));
  return result;
}
#endif
