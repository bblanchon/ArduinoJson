// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonArrayNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonArrayIterator {
 public:
  explicit JsonArrayIterator(Internals::JsonArrayNode *node) : _node(node) {}

  JsonValueImpl &operator*() const { return _node->value; }
  JsonValueImpl *operator->() { return &_node->value; }

  bool operator==(const JsonArrayIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _node != other._node;
  }

  JsonArrayIterator &operator++() {
    _node = _node->next;
    return *this;
  }

 private:
  JsonArrayNode *_node;
};
}
}
