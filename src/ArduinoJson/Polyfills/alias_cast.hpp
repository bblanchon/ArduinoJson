// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <stdint.h>
#include <stdlib.h>  // for size_t
#include "../Configuration.hpp"
#include "../Polyfills/math.hpp"

namespace ArduinoJson {
namespace Internals {

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

}  // namespace Internals
}  // namespace ArduinoJson
