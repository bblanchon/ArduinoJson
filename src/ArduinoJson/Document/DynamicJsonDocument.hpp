// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/BasicJsonDocument.hpp>

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

// The allocator of DynamicJsonDocument.
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

// A JsonDocument with a memory pool in the heap.
// https://arduinojson.org/v6/api/dynamicjsondocument/
typedef BasicJsonDocument<DefaultAllocator> DynamicJsonDocument;

}  // namespace ARDUINOJSON_NAMESPACE
