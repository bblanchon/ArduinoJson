// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonPair.hpp"
#include "JsonObjectNode.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonObjectConstIterator {
 public:
  explicit JsonObjectConstIterator(Internals::JsonObjectNode *node)
      : _pair(node) {}

  const JsonPair operator*() const { return _pair; }
  const JsonPair *operator->() { return &_pair; }

  bool operator==(const JsonObjectConstIterator &other) const {
    return _pair._node == other._pair._node;
  }

  bool operator!=(const JsonObjectConstIterator &other) const {
    return _pair._node != other._pair._node;
  }

  JsonObjectConstIterator &operator++() {
    _pair._node = _pair._node->next;
    return *this;
  }

 private:
  JsonPair _pair;
};
}
}
