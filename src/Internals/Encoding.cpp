// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../../include/ArduinoJson/Internals/Encoding.hpp"

// How to escape special chars:
// _escapeTable[2*i+1] => the special char
// _escapeTable[2*i] => the char to use instead
const char ArduinoJson::Internals::Encoding::_escapeTable[] =
    "\"\"\\\\b\bf\fn\nr\rt\t";
