#pragma once

#include "JsonNode.hpp"

namespace ArduinoJson {
class JsonValue;

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
