// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {
class Unparsed {
 public:
  explicit Unparsed(const char* str) : _str(str) {}
  operator const char*() const { return _str; }

 private:
  const char* _str;
};
}
}
