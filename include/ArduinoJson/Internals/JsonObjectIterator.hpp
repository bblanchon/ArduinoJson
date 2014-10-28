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
  explicit JsonObjectIterator(Internals::JsonObjectNode *node) : _pair(node) {}

  JsonPair &operator*() { return _pair; }
  JsonPair *operator->() { return &_pair; }

  bool operator==(const JsonObjectIterator &other) const {
    return _pair._node == other._pair._node;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _pair._node != other._pair._node;
  }

  JsonObjectIterator &operator++() {
    if (_pair._node) _pair._node = _pair._node->next;
    return *this;
  }

 private:
  JsonPair _pair;
};
}
}
