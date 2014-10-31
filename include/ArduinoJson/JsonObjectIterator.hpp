// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonObjectNode.hpp"

namespace ArduinoJson {

class JsonObjectIterator {
 public:
  explicit JsonObjectIterator(Internals::JsonObjectNode *node) : _node(node) {}

  JsonPair &operator*() { return _node->pair; }
  JsonPair *operator->() { return &_node->pair; }

  bool operator==(const JsonObjectIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _node != other._node;
  }

  JsonObjectIterator &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  Internals::JsonObjectNode *_node;
};
}
