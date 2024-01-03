// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include <ArduinoJson/Array/ArrayData.hpp>
#include <ArduinoJson/Numbers/JsonFloat.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>
#include <ArduinoJson/Object/ObjectData.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

enum {
  VALUE_MASK = 0x7F,

  OWNED_VALUE_BIT = 0x01,
  VALUE_IS_NULL = 0,
  VALUE_IS_RAW_STRING = 0x03,
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

union VariantContent {
  VariantContent() {}

  JsonFloat asFloat;
  bool asBoolean;
  JsonUInt asUnsignedInteger;
  JsonInteger asSignedInteger;
  ArrayData asArray;
  ObjectData asObject;
  CollectionData asCollection;
  const char* asLinkedString;
  struct StringNode* asOwnedString;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
