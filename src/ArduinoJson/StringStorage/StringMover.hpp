// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  class StringBuilder {
   public:
    typedef ZeroTerminatedRamStringConst StringType;

    StringBuilder(char** ptr) : _writePtr(ptr), _startPtr(*ptr) {}

    void append(char c) {
      *(*_writePtr)++ = char(c);
    }

    StringType complete() const {
      *(*_writePtr)++ = 0;
      return reinterpret_cast<const char*>(_startPtr);
    }

   private:
    char** _writePtr;
    char* _startPtr;
  };

  StringMover(char* ptr) : _ptr(ptr) {}

  StringBuilder startString() {
    return StringBuilder(&_ptr);
  }

 private:
  char* _ptr;
};
}  // namespace ARDUINOJSON_NAMESPACE
