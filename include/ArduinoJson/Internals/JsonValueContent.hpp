// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../ForwardDeclarations.hpp"

namespace ArduinoJson {
namespace Internals {

union JsonValueContent {
  bool asBoolean;
  double asDouble;
  long asLong;
  const char* asString;
  JsonArray* asArray;
  JsonObject* asObject;
};
}
}
