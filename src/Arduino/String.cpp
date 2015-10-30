// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#ifndef ARDUINO

#include "../../include/ArduinoJson/Arduino/String.hpp"

#include <stdio.h>  // for sprintf()

String::String(double value, unsigned char digits) {
  char tmp[32];
  sprintf(tmp, "%.*f", digits, value);
  *this = tmp;
}

String::String(long value) {
  char tmp[32];
  sprintf(tmp, "%ld", value);
  *this = tmp;
}

String::String(int value) {
  char tmp[32];
  sprintf(tmp, "%d", value);
  *this = tmp;
}

#endif
