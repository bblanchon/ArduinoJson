// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonValueInternal.hpp"

namespace ArduinoJson {
class JsonPair {
 public:
  const char *key() const { return _node->getAsObjectKey(); }

  JsonValue value() {
    return Internals::JsonValueInternal(_node->getAsObjectValue());
  }

 protected:
  explicit JsonPair(Internals::JsonNode *node) : _node(node) {}

  Internals::JsonNode *_node;
};
}
