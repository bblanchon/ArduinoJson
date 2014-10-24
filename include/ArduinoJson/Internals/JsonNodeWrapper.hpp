// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../ForwardDeclarations.hpp"
#include "JsonNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonNodeWrapper {
  friend class JsonValue;

 public:
  JsonNodeWrapper() : _node(0) {}

  explicit JsonNodeWrapper(JsonNode *node) : _node(node) {}

 protected:
  void duplicate(const JsonNodeWrapper &other) {
    if (!_node) {
      _node = other._node;
    } else {
      _node->duplicate(other._node);
    }
  }

  JsonNode *_node;
};
}
}
