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
  JsonSerializer(JsonWriter &writer) : _writer(writer) {}

  void serialize(JsonValueImpl *value);
  void serialize(JsonArrayImpl *value);
  void serialize(JsonObjectImpl *value);

 private:
  JsonWriter &_writer;
};
}
}