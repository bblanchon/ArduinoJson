// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
namespace Internals {

class JsonObjectIterator {
 public:
  explicit JsonObjectIterator(Internals::JsonObjectNode *node) : _node(node) {}

  JsonPair &operator*() const { return _node->pair; }
  JsonPair *operator->() { return &_node->pair; }

  bool operator==(const JsonObjectIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _node != other._node;
  }

  JsonObjectIterator &operator++() {
    _node = _node->next;
    return *this;
  }

 private:
  JsonObjectNode *_node;
};
}
}
