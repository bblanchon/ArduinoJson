// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <ArduinoJson.h>
#include <ostream>

namespace ArduinoJson {
std::ostream& operator<<(std::ostream& os, const JsonVariant& v);
std::ostream& operator<<(std::ostream& os, const JsonArray& v);
std::ostream& operator<<(std::ostream& os, const JsonObjectSubscript& v);
std::ostream& operator<<(std::ostream& os, const JsonArraySubscript& v);
}
