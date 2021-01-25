// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <stdint.h>
#include <stdlib.h>  // for size_t

#include <ArduinoJson/Configuration.hpp>
#include "math.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename F>
struct alias_cast_t {
  union {
    F raw;
    T data;
  };
};

template <typename T, typename F>
T alias_cast(F raw_data) {
  alias_cast_t<T, F> ac;
  ac.raw = raw_data;
  return ac.data;
}
}  // namespace ARDUINOJSON_NAMESPACE
