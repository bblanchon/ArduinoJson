// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include "type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename T>
inline void swap(T& a, T& b) {
  T t(a);
  a = b;
  b = t;
}

#if ARDUINOJSON_HAS_RVALUE_REFERENCES
template <typename T>
typename remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename remove_reference<T>::type&&>(t);
}
#else
template <typename T>
T& move(T& t) {
  return t;
}
#endif
}  // namespace ARDUINOJSON_NAMESPACE
