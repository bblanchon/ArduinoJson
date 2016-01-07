// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../../include/ArduinoJson/Internals/IndentedPrint.hpp"

using namespace ArduinoJson::Internals;

size_t IndentedPrint::write(uint8_t c) {
  size_t n = 0;

  if (isNewLine) n += writeTabs();

  n += sink->write(c);

  isNewLine = c == '\n';

  return n;
}

inline size_t IndentedPrint::writeTabs() {
  size_t n = 0;

  for (int i = 0; i < level * tabSize; i++) n += sink->write(' ');

  return n;
}
