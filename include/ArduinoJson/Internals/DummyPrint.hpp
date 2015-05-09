// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

// A dummy Print implementation used in JsonPrintable::measureLength()
class DummyPrint : public Print {
 public:
  virtual size_t write(uint8_t) { return 1; }
};
}
}
