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
class StringReader {
 public:
  StringReader(const char *input) : _ptr(input ? input : "") {}

  void skip() {
    _ptr++;
  }

  char peek() const {
    return _ptr[0];
  }

  char peekNext() const {
    return _ptr[1];
  }

 private:
  const char *_ptr;
};
}
}
