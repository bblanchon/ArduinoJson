// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>
#include <stdlib.h>  // for size_t

#include <ArduinoJson/Configuration.hpp>
#include "math.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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

ARDUINOJSON_END_PRIVATE_NAMESPACE
