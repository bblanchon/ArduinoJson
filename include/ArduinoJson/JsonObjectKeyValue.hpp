// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "JsonValue.hpp"

namespace ArduinoJson {
class JsonObjectKeyValue {
  friend class JsonObjectIterator;

 public:
  const char *key() const { return _node->getAsObjectKey(); }

  JsonValue value() { return JsonValue(_node->getAsObjectValue()); }

  bool operator==(const JsonObjectKeyValue &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectKeyValue &other) const {
    return _node != other._node;
  }

 private:
  explicit JsonObjectKeyValue(Internals::JsonNode *node) : _node(node) {}

  Internals::JsonNode *_node;
};
}
