// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonValueImpl.hpp"
#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonObjectNode {
 public:
  static JsonObjectNode* createFrom(JsonBuffer* buffer, const char* key) {
    void* ptr = buffer->alloc(sizeof(JsonObjectNode));
    return ptr ? new (ptr) JsonObjectNode(key) : NULL;
  }

  const char* const key;
  JsonValueImpl value;
  JsonObjectNode* next;

 private:
  JsonObjectNode(const char* k) : key(k) {}
};
}
}
