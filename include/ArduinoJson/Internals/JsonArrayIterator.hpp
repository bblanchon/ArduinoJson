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
  explicit JsonArrayIterator(Internals::JsonArrayNode *node) : _node(node) {
    updateValue();
  }

  JsonValue operator*() const { return _value; }
  JsonValue *operator->() { return &_value; }

  bool operator==(const JsonArrayIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _node != other._node;
  }

  JsonArrayIterator &operator++() {
    _node = _node->next;
    updateValue();
    return *this;
  }

 private:
  void updateValue() { _value = JsonValue(_node ? &_node->value : NULL); }

  JsonArrayNode *_node;
  JsonValue _value;
};
}
}
