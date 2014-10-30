// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonPair.hpp"
#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonObjectNode {
 public:
  JsonObjectNode(const char* k) : pair(k), next(NULL) {}

  JsonPair pair;
  JsonObjectNode* next;
};
}
}
