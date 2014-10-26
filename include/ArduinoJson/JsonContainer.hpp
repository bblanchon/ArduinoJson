// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Arduino/Printable.hpp"

namespace ArduinoJson {

class JsonContainer : public Printable {
 public:
  size_t printTo(char *buffer, size_t bufferSize) const;
  virtual size_t printTo(Print &print) const;

  size_t prettyPrintTo(char *buffer, size_t bufferSize) const;
  size_t prettyPrintTo(ArduinoJson::Internals::IndentedPrint &print) const;
  size_t prettyPrintTo(Print &print) const;
};
}
