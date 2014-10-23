// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ArduinoJson/JsonValue.hpp"

namespace ArduinoJson {
class JsonObjectKeyValue {
 public:
  explicit JsonObjectKeyValue(Internals::JsonNode *node) : _node(node) {}

  const char *key() const { return _node->getAsObjectKey(); }

  JsonValue value() { return JsonValue(_node->getAsObjectValue()); }

  bool operator==(const JsonObjectKeyValue &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectKeyValue &other) const {
    return _node != other._node;
  }

  Internals::JsonNode *next() { return _node->next; }

 private:
  Internals::JsonNode *_node;
};
}