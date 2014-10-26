// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonArrayNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonArrayConstIterator {
 public:
  explicit JsonArrayConstIterator(JsonArrayNode *node) : _node(node) {}

  const JsonValueImpl &operator*() const { return _node->value; }
  const JsonValueImpl *operator->() { return &_node->value; }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return _node != other._node;
  }

  JsonArrayConstIterator &operator++() {
    _node = _node->next;
    return *this;
  }

 private:
  JsonArrayNode *_node;
};
}
}
