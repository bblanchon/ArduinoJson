// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonObjectNode.hpp"

namespace ArduinoJson {

class JsonObjectConstIterator {
 public:
  explicit JsonObjectConstIterator(Internals::JsonObjectNode *node)
      : _node(node) {}

  const JsonPair &operator*() { return _node->content; }
  const JsonPair *operator->() { return &_node->content; }

  bool operator==(const JsonObjectConstIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectConstIterator &other) const {
    return _node != other._node;
  }

  JsonObjectConstIterator &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  Internals::JsonObjectNode *_node;
};
}
