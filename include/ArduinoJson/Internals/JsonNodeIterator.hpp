// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonNode.hpp"

namespace ArduinoJson {
namespace Internals {
// TODO: replace by JsonArrayIterator and JsonObjectIterator
class JsonNodeIterator {
 public:
  explicit JsonNodeIterator(JsonNode *node) : _node(node) {}

  bool operator!=(const JsonNodeIterator &other) const {
    return _node != other._node;
  }

  void operator++() { _node = _node->next; }

  JsonNode *operator*() const { return _node; }

  JsonNode *operator->() const { return _node; }

 private:
  JsonNode *_node;
};
}
}
