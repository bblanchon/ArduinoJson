// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "Printers.hpp"

std::ostream& ArduinoJson::operator<<(std::ostream& os,
                                      const ArduinoJson::JsonValue&) {
  os << "JsonValue";  // TODO
  return os;
}
