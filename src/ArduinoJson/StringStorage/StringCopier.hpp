// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringCopier {
 public:
  typedef ARDUINOJSON_NAMESPACE::StringBuilder StringBuilder;

  StringCopier(MemoryPool* pool) : _pool(pool) {}

  StringBuilder startString() {
    return StringBuilder(_pool);
  }

  void reclaim(const char* s) {
    _pool->reclaimLastString(s);
  }

 private:
  MemoryPool* _pool;
};
}  // namespace ARDUINOJSON_NAMESPACE
