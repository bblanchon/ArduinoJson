// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Document/BasicJsonDocument.hpp>

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

struct DefaultAllocator {
  void* allocate(size_t size) {
    return malloc(size);
  }

  void deallocate(void* ptr) {
    free(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return realloc(ptr, new_size);
  }
};

typedef BasicJsonDocument<DefaultAllocator> DynamicJsonDocument;

}  // namespace ARDUINOJSON_NAMESPACE
