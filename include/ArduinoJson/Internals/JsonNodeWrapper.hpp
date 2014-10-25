// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonNodeWrapper {
 protected:
  JsonNodeWrapper() : _node(0) {}

  explicit JsonNodeWrapper(JsonNode *node) : _node(node) {}

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
