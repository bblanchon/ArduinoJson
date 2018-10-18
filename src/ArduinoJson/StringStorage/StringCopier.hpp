// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Memory/StringBuilder.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringCopier {
 public:
  typedef ARDUINOJSON_NAMESPACE::StringBuilder StringBuilder;

  StringCopier(MemoryPool* memoryPool) : _memoryPool(memoryPool) {}

  StringBuilder startString() {
    return StringBuilder(_memoryPool);
  }

 private:
  MemoryPool* _memoryPool;
};
}  // namespace ARDUINOJSON_NAMESPACE
