// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

#ifdef ARDUINO
// On embedded platform, we with use float instead of double to keep JsonVariant
// small (issue #134)
typedef float JsonFloat;
#else
typedef double JsonFloat;
#endif
}
}
