// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // ptrdiff_t, size_t

#include "JsonFloat.hpp"
#include "JsonInteger.hpp"

namespace ARDUINOJSON_NAMESPACE {

enum JsonVariantType {
  JSON_NULL,
  JSON_LINKED_RAW,
  JSON_OWNED_RAW,
  JSON_LINKED_STRING,
  JSON_OWNED_STRING,
  JSON_BOOLEAN,
  JSON_POSITIVE_INTEGER,
  JSON_NEGATIVE_INTEGER,
  JSON_ARRAY,
  JSON_OBJECT,
  JSON_FLOAT
};

struct JsonObjectData {
  struct Slot *head;
  struct Slot *tail;
};

struct JsonArrayData {
  struct Slot *head;
  struct Slot *tail;
};

struct RawData {
  const char *data;
  size_t size;
};

// A union that defines the actual content of a JsonVariantData.
// The enum JsonVariantType determines which member is in use.
union JsonVariantContent {
  JsonFloat asFloat;
  JsonUInt asInteger;
  JsonArrayData asArray;
  JsonObjectData asObject;
  const char *asString;
  struct {
    const char *data;
    size_t size;
  } asRaw;
};

// this struct must be a POD type to prevent error calling offsetof on clang
struct JsonVariantData {
  bool keyIsStatic : 1;
  JsonVariantType type : 7;
  JsonVariantContent content;
};

inline JsonVariantData *getVariantData(JsonArrayData *arr) {
  const ptrdiff_t offset = offsetof(JsonVariantData, content) -
                           offsetof(JsonVariantContent, asArray);
  if (!arr) return 0;
  return reinterpret_cast<JsonVariantData *>(reinterpret_cast<char *>(arr) -
                                             offset);
}

inline JsonVariantData *getVariantData(JsonObjectData *obj) {
  const ptrdiff_t offset = offsetof(JsonVariantData, content) -
                           offsetof(JsonVariantContent, asObject);
  if (!obj) return 0;
  return reinterpret_cast<JsonVariantData *>(reinterpret_cast<char *>(obj) -
                                             offset);
}
}  // namespace ARDUINOJSON_NAMESPACE
