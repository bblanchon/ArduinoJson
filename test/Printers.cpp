// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "Printers.hpp"

std::ostream& operator<<(std::ostream& os, const ArduinoJson::JsonValue& v) {
  os << "JsonValue";  // TODO
  return os;
}