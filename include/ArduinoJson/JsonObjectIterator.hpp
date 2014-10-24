// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "JsonObjectKeyValue.hpp"

namespace ArduinoJson {

class JsonObjectIterator {
  friend class JsonObject;

 public:
  JsonObjectKeyValue operator*() const { return _keyValue; }
  JsonObjectKeyValue *operator->() { return &_keyValue; }

  bool operator==(const JsonObjectIterator &other) const {
      return _keyValue._node == other._keyValue._node;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _keyValue._node != other._keyValue._node;
  }

  JsonObjectIterator &operator++() {
    _keyValue._node = _keyValue._node->next;
    return *this;
  }

 private:
  explicit JsonObjectIterator(Internals::JsonNode *node) : _keyValue(node) {}

  JsonObjectKeyValue _keyValue;
};
}
