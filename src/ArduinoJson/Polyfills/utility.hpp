// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include "type_traits.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

using nullptr_t = decltype(nullptr);

template <class T>
T&& forward(typename remove_reference<T>::type& t) noexcept {
  return static_cast<T&&>(t);
}

template <class T>
typename remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename remove_reference<T>::type&&>(t);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
