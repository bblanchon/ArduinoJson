// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  class StringBuilder {
   public:
    StringBuilder(char** ptr) : _writePtr(ptr), _startPtr(*ptr) {}

    void append(char c) {
      *(*_writePtr)++ = char(c);
    }

    char* complete() const {
      *(*_writePtr)++ = 0;
      return _startPtr;
    }

   private:
    char** _writePtr;
    char* _startPtr;
  };

  StringMover(char* ptr) : _ptr(ptr) {}

  StringBuilder startString() {
    return StringBuilder(&_ptr);
  }

  // recover memory from last string
  void reclaim(const char* str) {
    _ptr = const_cast<char*>(str);
  }

 private:
  char* _ptr;
};
}  // namespace ARDUINOJSON_NAMESPACE
