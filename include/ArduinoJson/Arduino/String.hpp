// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifndef ARDUINO

#include <string>

// This class reproduces Arduino's String class
class String : public std::string {
 public:
  String(const char *cstr = "") : std::string(cstr) {}
  String(const String &str) : std::string(str) {}
  explicit String(char c);
  explicit String(unsigned char);
  explicit String(int);
  explicit String(unsigned int);
  explicit String(long);
  explicit String(unsigned long);
  explicit String(float, unsigned char decimalPlaces = 2);
  explicit String(double, unsigned char decimalPlaces = 2);
};

#else

#include <WString.h>

#endif
