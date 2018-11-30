// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonDocument.hpp"

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

class DynamicJsonDocument : public JsonDocument {
 public:
  DynamicJsonDocument(size_t capa = ARDUINOJSON_DEFAULT_POOL_SIZE)
      : JsonDocument(alloc(capa), addPadding(capa)) {}

  DynamicJsonDocument(const DynamicJsonDocument& src)
      : JsonDocument(alloc(src.memoryUsage()), addPadding(src.memoryUsage())) {
    copy(src);
  }

  DynamicJsonDocument(const JsonDocument& src)
      : JsonDocument(alloc(src.memoryUsage()), addPadding(src.memoryUsage())) {
    copy(src);
  }

  ~DynamicJsonDocument() {
    free(memoryPool().buffer());
  }

  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    copy(src);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const JsonDocument& src) {
    copy(src);
    return *this;
  }

 private:
  static char* alloc(size_t capa) {
    return reinterpret_cast<char*>(malloc(addPadding(capa)));
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
