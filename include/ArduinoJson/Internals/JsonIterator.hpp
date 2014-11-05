// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TNode, typename TValue>
class JsonIterator {
 public:
  explicit JsonIterator(TNode *node) : _node(node) {}

  TValue &operator*() const { return _node->content; }
  TValue *operator->() { return &_node->content; }

  bool operator==(const JsonIterator<TNode, TValue> &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonIterator<TNode, TValue> &other) const {
    return _node != other._node;
  }

  JsonIterator<TNode, TValue> &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

 private:
  TNode *_node;
};
}
}
