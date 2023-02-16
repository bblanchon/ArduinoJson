// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

enum {
  VALUE_MASK = 0x7F,

  OWNED_VALUE_BIT = 0x01,
  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW = 0x02,
  VALUE_IS_OWNED_RAW = 0x03,
  VALUE_IS_LINKED_STRING = 0x04,
  VALUE_IS_OWNED_STRING = 0x05,

  // CAUTION: no OWNED_VALUE_BIT below

  VALUE_IS_BOOLEAN = 0x06,

  NUMBER_BIT = 0x08,
  VALUE_IS_UNSIGNED_INTEGER = 0x08,
  VALUE_IS_SIGNED_INTEGER = 0x0A,
  VALUE_IS_FLOAT = 0x0C,

  COLLECTION_MASK = 0x60,
  VALUE_IS_OBJECT = 0x20,
  VALUE_IS_ARRAY = 0x40,

  OWNED_KEY_BIT = 0x80
};

struct RawData {
  const char* data;
  size_t size;
};

union VariantContent {
  JsonFloat asFloat;
  bool asBoolean;
  JsonUInt asUnsignedInteger;
  JsonInteger asSignedInteger;
  CollectionData asCollection;
  struct {
    const char* data;
    size_t size;
  } asString;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
