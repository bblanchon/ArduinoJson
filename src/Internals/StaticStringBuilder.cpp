// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../../include/ArduinoJson/Internals/StaticStringBuilder.hpp"

using namespace ArduinoJson::Internals;

size_t StaticStringBuilder::write(uint8_t c) {
  if (length >= capacity) return 0;

  buffer[length++] = c;
  buffer[length] = '\0';
  return 1;
}
