// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"

#if ARDUINOJSON_USE_ARDUINO_STRING

#include <WString.h>

#else

#include <string>

namespace ArduinoJson {
typedef std::string String;
}

#endif
