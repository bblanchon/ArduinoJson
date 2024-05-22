// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>  // int8_t, int16_t

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <int Bits>
struct uint_;

template <>
struct uint_<8> {
  typedef uint8_t type;
};

template <>
struct uint_<16> {
  typedef uint16_t type;
};

template <>
struct uint_<32> {
  typedef uint32_t type;
};

template <int Bits>
using uint_t = typename uint_<Bits>::type;

ARDUINOJSON_END_PRIVATE_NAMESPACE
