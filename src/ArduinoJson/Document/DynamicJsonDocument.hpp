// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "JsonDocument.hpp"

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

class DynamicJsonDocument : public JsonDocument {
 public:
  explicit DynamicJsonDocument(size_t capa) : JsonDocument(allocPool(capa)) {}

  DynamicJsonDocument(const DynamicJsonDocument& src)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  template <typename T>
  DynamicJsonDocument(const T& src,
                      typename enable_if<IsVisitable<T>::value>::type* = 0)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  // disambiguate
  DynamicJsonDocument(VariantRef src)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  ~DynamicJsonDocument() {
    freePool();
  }

  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const T& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }

 private:
  MemoryPool allocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
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
