// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include "../Collection/CollectionData.hpp"
#include "../Numbers/Float.hpp"
#include "../Numbers/Integer.hpp"

namespace ARDUINOJSON_NAMESPACE {

//
enum {
  VALUE_MASK = 0x7F,

  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW = 0x01,
  VALUE_IS_OWNED_RAW = 0x02,
  VALUE_IS_LINKED_STRING = 0x03,
  VALUE_IS_OWNED_STRING = 0x04,
  VALUE_IS_BOOLEAN = 0x05,
  VALUE_IS_POSITIVE_INTEGER = 0x06,
  VALUE_IS_NEGATIVE_INTEGER = 0x07,
  VALUE_IS_FLOAT = 0x08,

  COLLECTION_MASK = 0x60,
  VALUE_IS_OBJECT = 0x20,
  VALUE_IS_ARRAY = 0x40,

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
