// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>  // int8_t, int16_t

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <int Bits>
struct uint_t;

template <>
struct uint_t<8> {
  typedef uint8_t type;
};

template <>
struct uint_t<16> {
  typedef uint16_t type;
};

template <>
struct uint_t<32> {
  typedef uint32_t type;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
