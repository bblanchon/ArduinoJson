// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/String.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  StringMover(char* ptr) : _writePtr(ptr) {}

  void startString() {
    _startPtr = _writePtr;
  }

  FORCE_INLINE String save() {
    String s = str();
    _writePtr++;
    return s;
  }

  void append(char c) {
    *_writePtr++ = c;
  }

  bool isValid() const {
    return true;
  }

  String str() const {
    _writePtr[0] = 0;  // terminator
    return String(_startPtr, size(), true);
  }

  size_t size() const {
    return size_t(_writePtr - _startPtr);
  }

 private:
  char* _writePtr;
  char* _startPtr;
};
}  // namespace ARDUINOJSON_NAMESPACE
