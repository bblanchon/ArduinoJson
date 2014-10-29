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
  static JsonArrayNode* createFrom(JsonBuffer* buffer) {
    void* ptr = buffer->alloc(sizeof(JsonArrayNode));
    return ptr ? new (ptr) JsonArrayNode() : NULL;
  }

  JsonArrayNode* next;
  JsonValue value;

 private:
  JsonArrayNode() : next(0) {}
};
}
}
