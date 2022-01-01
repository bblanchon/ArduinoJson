// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoredString.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  typedef LinkedString string_type;

  StringMover(char* ptr) : _writePtr(ptr) {}

  void startString() {
    _startPtr = _writePtr;
  }

  FORCE_INLINE string_type save() {
    _writePtr[0] = 0;  // terminator
    string_type s = str();
    _writePtr++;
    return s;
  }

  void append(char c) {
    *_writePtr++ = c;
  }

  bool isValid() const {
    return true;
  }

  string_type str() const {
    return string_type(_startPtr, size());
  }

  size_t size() const {
    return size_t(_writePtr - _startPtr);
  }

 private:
  char* _writePtr;
  char* _startPtr;
};
}  // namespace ARDUINOJSON_NAMESPACE
