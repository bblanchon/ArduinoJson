// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#if ARDUINOJSON_ENABLE_PROGMEM
#  include <ArduinoJson/Polyfills/pgmspace.hpp>
#  include <ArduinoJson/Polyfills/type_traits.hpp>
#endif

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_ENABLE_PROGMEM

#  ifndef ARDUINOJSON_DEFINE_PROGMEM_ARRAY
#    define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, value) \
      static type const name[] PROGMEM = value;
#  endif

template <typename T>
typename enable_if<is_pointer<T>::value, T>::type pgm_read(const void* p) {
  return reinterpret_cast<T>(pgm_read_ptr(p));
}

template <typename T>
typename enable_if<is_same<T, uint32_t>::value, T>::type pgm_read(
    const void* p) {
  return pgm_read_dword(p);
}
#else

#  ifndef ARDUINOJSON_DEFINE_PROGMEM_ARRAY
#    define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, value) \
      static type const name[] = value;
#  endif

template <typename T>
inline T pgm_read(const void* p) {
  return *reinterpret_cast<const T*>(p);
}

#endif

}  // namespace ARDUINOJSON_NAMESPACE
