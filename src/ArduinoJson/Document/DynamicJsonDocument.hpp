// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonDocument.hpp"

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

class DynamicJsonDocument : public JsonDocument {
 public:
  explicit DynamicJsonDocument(size_t capa)
      : JsonDocument(allocPool(addPadding(capa))) {}

  DynamicJsonDocument(const DynamicJsonDocument& src)
      : JsonDocument(allocPool(src.capacity())) {
    copy(src);
  }

  DynamicJsonDocument(const JsonDocument& src)
      : JsonDocument(allocPool(src.capacity())) {
    copy(src);
  }

  ~DynamicJsonDocument() {
    freePool();
  }

  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    copy(src);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const JsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    copy(src);
    return *this;
  }

 private:
  MemoryPool allocPool(size_t capa) {
    return MemoryPool(reinterpret_cast<char*>(malloc(capa)), capa);
  }

  void reallocPoolIfTooSmall(size_t requiredSize) {
    if (requiredSize <= capacity()) return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }

  void freePool() {
    free(memoryPool().buffer());
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
