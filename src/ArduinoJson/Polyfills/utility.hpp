// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include "type_traits.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <class T>
T&& forward(typename remove_reference<T>::type& t) noexcept {
  return static_cast<T&&>(t);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
