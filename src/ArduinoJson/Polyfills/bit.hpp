// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdlib.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename To, typename From>
To bit_cast(const From& src) {
  static_assert(sizeof(To) == sizeof(From), "size mismatch");
  To dst;
  ::memcpy(&dst, &src, sizeof(From));
  return dst;
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
