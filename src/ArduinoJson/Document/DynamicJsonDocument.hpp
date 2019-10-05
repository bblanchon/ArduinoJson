// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Document/BasicJsonDocument.hpp>

#include <stdlib.h>  // malloc, free

namespace ARDUINOJSON_NAMESPACE {

struct DefaultAllocator {
  void* allocate(size_t n) {
    return malloc(n);
  }

  void deallocate(void* p) {
    free(p);
  }
};

typedef BasicJsonDocument<DefaultAllocator> DynamicJsonDocument;

}  // namespace ARDUINOJSON_NAMESPACE
