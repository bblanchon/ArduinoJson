// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonValue.hpp"
#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonArrayNode {
 public:
  JsonArrayNode() : next(NULL) {}

  JsonArrayNode* next;
  JsonValue value;
};
}
}
