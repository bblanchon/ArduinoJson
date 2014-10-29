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
  static JsonObjectNode* createFrom(JsonBuffer* buffer, const char* key) {
    void* ptr = buffer->alloc(sizeof(JsonObjectNode));
    return ptr ? new (ptr) JsonObjectNode(key) : NULL;
  }

  JsonPair pair;
  JsonObjectNode* next;

 private:
  JsonObjectNode(const char* k) : pair(k), next(NULL) {}
};
}
}
