// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
class StringWriter {
 public:
  StringWriter(char *buffer) : _ptr(buffer) {}

  const char *startString() {
    return _ptr;
  }

  void stopString() {
    *_ptr++ = 0;
  }

  void append(char c) {
    *_ptr++ = c;
  }

 private:
  char *_ptr;
};
}
}
