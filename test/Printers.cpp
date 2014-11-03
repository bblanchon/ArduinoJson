// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "Printers.hpp"

std::ostream& ArduinoJson::operator<<(std::ostream& os,
                                      const ArduinoJson::JsonValue& v) {
  if (v.is<long>())
    os << v.as<long>();
  else if (v.is<double>())
    os << v.as<double>();
  else
    os << "JsonValue";  // TODO
  return os;
}
