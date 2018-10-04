// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <typename TMemoryPool>
class StringCopier {
 public:
  StringCopier(TMemoryPool& memoryPool) : _memoryPool(&memoryPool) {}

  typedef typename TMemoryPool::StringBuilder StringBuilder;

  StringBuilder startString() {
    return _memoryPool->startString();
  }

 private:
  TMemoryPool* _memoryPool;
};
}  // namespace ARDUINOJSON_NAMESPACE
