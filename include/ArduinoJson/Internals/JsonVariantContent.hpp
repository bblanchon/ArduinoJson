// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {

class JsonArray;
class JsonObject;

namespace Internals {

union JsonVariantContent {
  bool asBoolean;
  double asDouble;
  long asLong;
  const char* asString;
  JsonArray* asArray;
  JsonObject* asObject;
};
}
}
