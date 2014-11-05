// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Node.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename T>
class NodeIterator {
 public:
  explicit NodeIterator(Node<T> *node = NULL) : _node(node) {}

  T &operator*() const { return _node->content; }
  T *operator->() { return &_node->content; }

  bool operator==(const NodeIterator<T> &other) const {
    return _node == other._node;
  }

  bool operator!=(const NodeIterator<T> &other) const {
    return _node != other._node;
  }

  NodeIterator<T> &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  Node<T> *_node;
};
}
}
