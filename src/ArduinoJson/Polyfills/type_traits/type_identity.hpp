// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include "integral_constant.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct type_identity {
  using type = T;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
