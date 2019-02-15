// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include "../Configuration.hpp"

#define JSON_STRING_SIZE(SIZE) (SIZE)

namespace ARDUINOJSON_NAMESPACE {

struct StringSlot {
  char *value;
  size_t size;
};
}  // namespace ARDUINOJSON_NAMESPACE
