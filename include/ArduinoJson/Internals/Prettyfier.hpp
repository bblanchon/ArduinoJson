// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {

// Converts a compact JSON string into an indented one.
class Prettyfier : public Print {
 public:
  explicit Prettyfier(IndentedPrint& p) : _sink(p) {
    _previousChar = 0;
    _inString = false;
  }

  virtual size_t write(uint8_t);

 private:
  Prettyfier& operator=(const Prettyfier&);  // cannot be assigned

  bool inEmptyBlock() { return _previousChar == '{' || _previousChar == '['; }

  size_t handleStringChar(uint8_t);
  size_t handleMarkupChar(uint8_t);

  size_t handleBlockClose(uint8_t);
  size_t handleBlockOpen(uint8_t);
  size_t handleColumn();
  size_t handleComma();
  size_t handleQuoteOpen();
  size_t handleNormalChar(uint8_t);
  size_t indentIfNeeded();
  size_t unindentIfNeeded();

  uint8_t _previousChar;
  IndentedPrint& _sink;
  bool _inString;
};
}
}
