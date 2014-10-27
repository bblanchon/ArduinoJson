// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonValue.hpp"
#include "Internals/JsonObjectNode.hpp"

namespace ArduinoJson {
class JsonPair {
  friend class Internals::JsonObjectIterator;
  friend class Internals::JsonObjectConstIterator;

 public:
  JsonPair(Internals::JsonObjectNode *node) : _node(node) {}

  const char *key() const { return _node->key; }
  JsonValue value() { return JsonValue(&_node->value); }

 private:
  Internals::JsonObjectNode *_node;
};
}
