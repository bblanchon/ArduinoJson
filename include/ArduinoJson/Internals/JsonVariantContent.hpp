// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {

// Forward declarations
class JsonArray;
class JsonObject;

namespace Internals {

// A union that defines the actual content of a JsonVariant.
// The enum JsonVariantType determines which member is in use.
union JsonVariantContent {
  bool asBoolean;
  double asDouble;       // asDouble is also used for float
  long asLong;           // asLong is also used for char, short and int
  const char* asString;  // asString can be null
  JsonArray* asArray;    // asArray cannot be null
  JsonObject* asObject;  // asObject cannot be null
};
}
}
