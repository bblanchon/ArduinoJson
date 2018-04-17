// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdlib.h>  // for size_t

namespace ArduinoJson {
namespace Internals {

// A meta-function that returns the highest value
template <size_t X, size_t Y, bool MaxIsX = (X > Y)>
struct Max {};

template <size_t X, size_t Y>
struct Max<X, Y, true> {
  static const size_t value = X;
};

template <size_t X, size_t Y>
struct Max<X, Y, false> {
  static const size_t value = Y;
};
}  // namespace Internals
}  // namespace ArduinoJson
