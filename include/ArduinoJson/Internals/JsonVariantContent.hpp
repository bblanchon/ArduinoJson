// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonFloat.hpp"
#include "JsonInteger.hpp"

namespace ArduinoJson {

// Forward declarations
class JsonArray;
class JsonObject;

namespace Internals {
// A union that defines the actual content of a JsonVariant.
// The enum JsonVariantType determines which member is in use.
union JsonVariantContent {
  JsonFloat asFloat;      // used for double and float
  JsonInteger asInteger;  // used for bool, char, short, int and longs
  const char* asString;   // asString can be null
  JsonArray* asArray;     // asArray cannot be null
  JsonObject* asObject;   // asObject cannot be null
};
}
}
