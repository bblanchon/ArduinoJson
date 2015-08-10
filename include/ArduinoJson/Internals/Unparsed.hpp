// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

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
