// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <ArduinoJson/JsonVariant.hpp>
#include <ostream>

namespace ArduinoJson {
std::ostream& operator<<(std::ostream& os, const ArduinoJson::JsonVariant& v);
std::ostream& operator<<(std::ostream& os, const ArduinoJson::JsonArray& v);
}
