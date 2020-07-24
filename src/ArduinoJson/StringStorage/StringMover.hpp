// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  StringMover(char* ptr) : _writePtr(ptr) {}

  void startString(MemoryPool*) {
    _startPtr = _writePtr;
  }

  const char* save(MemoryPool*) const {
    return _startPtr;
  }

  void append(char c) {
    *_writePtr++ = c;
  }

  bool isValid() const {
    return true;
  }

  const char* c_str() const {
    return _startPtr;
  }

  typedef storage_policies::store_by_address storage_policy;

 private:
  char* _writePtr;
  char* _startPtr;
};
}  // namespace ARDUINOJSON_NAMESPACE
