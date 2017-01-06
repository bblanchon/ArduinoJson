// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

class StringWriter {
 public:
  class String {
   public:
    String(char** ptr) : _writePtr(ptr), _startPtr(*ptr) {}

    void append(char c) {
      *(*_writePtr)++ = c;
    }

    const char* c_str() const {
      *(*_writePtr)++ = 0;
      return _startPtr;
    }

   private:
    char** _writePtr;
    char* _startPtr;
  };

  StringWriter(char* buffer) : _ptr(buffer) {}

  String startString() {
    return String(&_ptr);
  }

 private:
  char* _ptr;
};
}
}
