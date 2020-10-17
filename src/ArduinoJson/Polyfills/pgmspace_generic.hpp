// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
typename enable_if<is_pointer<T>::value, T>::type pgm_read(const void* p) {
  return reinterpret_cast<T>(pgm_read_ptr(p));
}

template <typename T>
typename enable_if<is_floating_point<T>::value &&
                       sizeof(T) == sizeof(float),  // on AVR sizeof(double) ==
                                                    // sizeof(float)
                   T>::type
pgm_read(const void* p) {
  return pgm_read_float(p);
}

template <typename T>
typename enable_if<is_same<T, uint32_t>::value, T>::type pgm_read(
    const void* p) {
  return pgm_read_dword(p);
}

}  // namespace ARDUINOJSON_NAMESPACE
