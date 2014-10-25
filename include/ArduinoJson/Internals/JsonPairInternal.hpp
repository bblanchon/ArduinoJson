// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonPair.hpp"

namespace ArduinoJson {
class JsonPairInternal : public JsonPair {
 public:
  explicit JsonPairInternal(Internals::JsonNode* node) : JsonPair(node) {}

  void moveToNext() { _node = _node->next; }

  bool isSameAs(const JsonPairInternal& other) const {
    return _node == other._node;
  }
};
}
