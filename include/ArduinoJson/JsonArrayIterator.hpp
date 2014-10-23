// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ArduinoJson/JsonValue.hpp"

namespace ArduinoJson {
class JsonArray;

class JsonArrayIterator {
  friend class JsonArray;

 public:
  explicit JsonArrayIterator(Internals::JsonNode *node) : _node(node) {}

  void operator++() { _node = _node->next; }

  JsonValue operator*() const { return JsonValue(_node); }

  bool operator==(const JsonArrayIterator &other) const {
    return _node == other._node;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _node != other._node;
  }

 private:
  Internals::JsonNode *_node;
};
}