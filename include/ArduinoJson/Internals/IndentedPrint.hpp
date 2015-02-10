// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

// Decorator on top of Print to allow indented output.
// This class is used by JsonPrintable::prettyPrintTo() but can also be used
// for your own purpose, like logging.
class IndentedPrint : public Print {
 public:
  explicit IndentedPrint(Print &p) : sink(&p) {
    level = 0;
    tabSize = 2;
    isNewLine = true;
  }

  virtual size_t write(uint8_t);

  // Adds one level of indentation
  void indent() {
    if (level < MAX_LEVEL) level++;
  }

  // Removes one level of indentation
  void unindent() {
    if (level > 0) level--;
  }

  // Set the number of space printed for each level of indentation
  void setTabSize(uint8_t n) {
    if (n < MAX_TAB_SIZE) tabSize = n & MAX_TAB_SIZE;
  }

 private:
  Print *sink;
  uint8_t level : 4;
  uint8_t tabSize : 3;
  bool isNewLine : 1;

  size_t writeTabs();

  static const int MAX_LEVEL = 15;    // because it's only 4 bits
  static const int MAX_TAB_SIZE = 7;  // because it's only 3 bits
};
}
}
