// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonValue.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonValueInternal : public JsonValue {
 public:
  explicit JsonValueInternal(Internals::JsonNode* node) : JsonValue(node) {}

  void moveToNext() { _node = _node->next; }

  bool isSameAs(const JsonValueInternal& other) const {
    return _node == other._node;
  }
};
}
}
