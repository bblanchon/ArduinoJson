// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"
#include "../Arduino/String.hpp"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a String
class DynamicStringBuilder : public Print {
 public:
  DynamicStringBuilder(String &str) : _str(str) {}

  virtual size_t write(uint8_t c) {
    _str += c;
    return 1;
  }

 private:
  DynamicStringBuilder &operator=(const DynamicStringBuilder &);

  String &_str;
};
}
}
