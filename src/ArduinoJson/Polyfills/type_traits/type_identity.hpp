// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include "integral_constant.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct type_identity {
  typedef T type;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
