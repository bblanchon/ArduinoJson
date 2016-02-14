// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

namespace ArduinoJson {
namespace Internals {

#if ARDUINOJSON_USE_LONG_LONG
typedef long long JsonInteger;
#elif ARDUINOJSON_USE_INT64
typedef __int64 JsonInteger;
#else
typedef long JsonInteger;
#endif
}
}
