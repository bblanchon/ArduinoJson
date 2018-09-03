// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TMemoryPool>
class StringCopier {
 public:
  StringCopier(TMemoryPool& memoryPool) : _memoryPool(&memoryPool) {}

  typedef typename TMemoryPool::String String;

  String startString() {
    return _memoryPool->startString();
  }

 private:
  TMemoryPool* _memoryPool;
};
}  // namespace Internals
}  // namespace ArduinoJson
