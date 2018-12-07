// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include "../Collection/CollectionData.hpp"
#include "../Numbers/Float.hpp"
#include "../Numbers/Integer.hpp"

namespace ARDUINOJSON_NAMESPACE {

enum {
  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW,
  VALUE_IS_OWNED_RAW,
  VALUE_IS_LINKED_STRING,
  VALUE_IS_OWNED_STRING,
  VALUE_IS_BOOLEAN,
  VALUE_IS_POSITIVE_INTEGER,
  VALUE_IS_NEGATIVE_INTEGER,
  VALUE_IS_ARRAY,
  VALUE_IS_OBJECT,
  VALUE_IS_FLOAT,
  VALUE_MASK = 0x7F,
  KEY_IS_OWNED = 0x80
};

struct RawData {
  const char *data;
  size_t size;
};

union VariantContent {
  Float asFloat;
  UInt asInteger;
  CollectionData asCollection;
  const char *asString;
  struct {
    const char *data;
    size_t size;
  } asRaw;
};
}  // namespace ARDUINOJSON_NAMESPACE
