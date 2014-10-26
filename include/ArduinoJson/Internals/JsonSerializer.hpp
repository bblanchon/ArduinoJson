// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../ForwardDeclarations.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonSerializer {
 public:
  static writeTo(JsonValue& value, JsonWriter&);

 private:
  inline void writeArrayTo(JsonValue& value, JsonWriter&);
  inline void writeObjectTo(JsonValue& value, JsonWriter&);
};
}
}