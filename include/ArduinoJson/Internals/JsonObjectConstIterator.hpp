// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonObjectNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonObjectConstIterator {
 public:
  explicit JsonObjectConstIterator(Internals::JsonObjectNode *node)
      : _node(node) {}

  JsonPair operator*() const { return _node->pair; }
  JsonPair *operator->() { return &_node->pair; }

  bool operator==(const JsonObjectConstIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectConstIterator &other) const {
    return _node != other._node;
  }

  JsonObjectConstIterator &operator++() {
    _node = _node->next;
    return *this;
  }

 private:
  JsonObjectNode *_node;
};
}
}
