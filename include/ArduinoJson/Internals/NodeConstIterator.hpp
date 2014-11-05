// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename T>
class NodeConstIterator {
 public:
  explicit NodeConstIterator(const Node<T> *node) : _node(node) {}

  const T &operator*() const { return _node->content; }
  const T *operator->() { return &_node->content; }

  bool operator==(const NodeConstIterator<T> &other) const {
    return _node == other._node;
  }

  bool operator!=(const NodeConstIterator<T> &other) const {
    return _node != other._node;
  }

  NodeConstIterator<T> &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  const Node<T> *_node;
};
}
}
