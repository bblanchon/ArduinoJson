// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonArrayNode.hpp"

namespace ArduinoJson {

class JsonArrayConstIterator {
 public:
  explicit JsonArrayConstIterator(Internals::JsonArrayNode *node)
      : _node(node) {}

  const JsonValue &operator*() const { return _node->value; }
  const JsonValue *operator->() { return &_node->value; }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return _node != other._node;
  }

  JsonArrayConstIterator &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  Internals::JsonArrayNode *_node;
};
}