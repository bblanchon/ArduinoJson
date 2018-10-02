// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariantContent.hpp"
#include "JsonVariantType.hpp"

namespace ARDUINOJSON_NAMESPACE {

// this struct must be a POD type to prevent error calling offsetof on clang
struct JsonVariantData {
  JsonVariantType type;
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
