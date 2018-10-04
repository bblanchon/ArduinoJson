// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"

#include <stdint.h>  // int64_t

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_USE_LONG_LONG
typedef int64_t JsonInteger;
typedef uint64_t JsonUInt;
#else
typedef long JsonInteger;
typedef unsigned long JsonUInt;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
