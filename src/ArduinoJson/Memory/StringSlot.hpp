// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include "../Configuration.hpp"

#define JSON_STRING_SIZE(SIZE) \
  (sizeof(ARDUINOJSON_NAMESPACE::StringSlot) + (SIZE))

namespace ARDUINOJSON_NAMESPACE {

struct StringSlot {
  char *value;
  size_t size;
  struct StringSlot *next;
};
}  // namespace ARDUINOJSON_NAMESPACE
