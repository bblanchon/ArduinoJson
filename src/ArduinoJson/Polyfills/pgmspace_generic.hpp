// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#if ARDUINOJSON_ENABLE_PROGMEM
#  include <ArduinoJson/Polyfills/pgmspace.hpp>
#  include <ArduinoJson/Polyfills/type_traits.hpp>
#endif

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

#if ARDUINOJSON_ENABLE_PROGMEM

#  ifndef ARDUINOJSON_DEFINE_PROGMEM_ARRAY
#    define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, ...) \
      static type const name[] PROGMEM = __VA_ARGS__;
#  endif

template <typename T>
inline const T* pgm_read(const T* const* p) {
  return reinterpret_cast<const T*>(pgm_read_ptr(p));
}

inline uint32_t pgm_read(const uint32_t* p) {
  return pgm_read_dword(p);
}

template <typename T>
inline T pgm_read(const T* p) {
  T result;
  memcpy_P(&result, p, sizeof(T));
  return result;
}

#else

#  ifndef ARDUINOJSON_DEFINE_PROGMEM_ARRAY
#    define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, ...) \
      static type const name[] = __VA_ARGS__;
#  endif

template <typename T>
inline T pgm_read(const T* p) {
  return *p;
}

#endif

ARDUINOJSON_END_PRIVATE_NAMESPACE
