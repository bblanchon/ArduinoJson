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
  OWNED_VALUE_BIT = 0x01,
  VALUE_IS_NULL = 0,
  VALUE_IS_RAW_STRING = 0x03,
  VALUE_IS_LINKED_STRING = 0x04,
  VALUE_IS_OWNED_STRING = 0x05,

  // CAUTION: no OWNED_VALUE_BIT below

  VALUE_IS_BOOLEAN = 0x06,

  NUMBER_BIT = 0x08,       // 0000 1000
  VALUE_IS_UINT32 = 0x0A,  // 0000 1010
  VALUE_IS_INT32 = 0x0C,   // 0000 1100
  VALUE_IS_FLOAT = 0x0E,   // 0000 1110

#if ARDUINOJSON_USE_EXTENSIONS
  EXTENSION_BIT = 0x10,  // 0001 0000
#endif
#if ARDUINOJSON_USE_LONG_LONG
  VALUE_IS_UINT64 = 0x1A,  // 0001 1010
  VALUE_IS_INT64 = 0x1C,   // 0001 1100
#endif
#if ARDUINOJSON_USE_DOUBLE
  VALUE_IS_DOUBLE = 0x1E,  // 0001 1110
#endif

  COLLECTION_MASK = 0x60,
  VALUE_IS_OBJECT = 0x20,
  VALUE_IS_ARRAY = 0x40,
};

union VariantContent {
  VariantContent() {}

  float asFloat;
  bool asBoolean;
  uint32_t asUint32;
  int32_t asInt32;
#if ARDUINOJSON_USE_EXTENSIONS
  SlotId asSlotId;
#endif
  ArrayData asArray;
  ObjectData asObject;
  CollectionData asCollection;
  const char* asLinkedString;
  struct StringNode* asOwnedString;
};

#if ARDUINOJSON_USE_EXTENSIONS
union VariantExtension {
#  if ARDUINOJSON_USE_LONG_LONG
  uint64_t asUint64;
  int64_t asInt64;
#  endif
#  if ARDUINOJSON_USE_DOUBLE
  double asDouble;
#  endif
};
#endif

ARDUINOJSON_END_PRIVATE_NAMESPACE
