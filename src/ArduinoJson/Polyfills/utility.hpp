// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "type_traits.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

using nullptr_t = decltype(nullptr);

template <class T>
T&& forward(remove_reference_t<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <class T>
remove_reference_t<T>&& move(T&& t) {
  return static_cast<remove_reference_t<T>&&>(t);
}

// Polyfull for std::swap
// Don't use the name "swap" because it makes calls ambiguous for types in the
// detail namespace
template <class T>
void swap_(T& a, T& b) {
  T tmp = move(a);
  a = move(b);
  b = move(tmp);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
