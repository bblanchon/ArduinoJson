// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <stdint.h>  // int8_t, int16_t

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

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
}  // namespace ARDUINOJSON_NAMESPACE
