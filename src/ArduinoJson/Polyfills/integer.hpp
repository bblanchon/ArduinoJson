// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>  // int8_t, int16_t

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <int Bits>
struct int_t;

template <>
struct int_t<8> {
  typedef int8_t type;
};

template <>
struct int_t<16> {
  typedef int16_t type;
};

template <>
struct int_t<32> {
  typedef int32_t type;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
