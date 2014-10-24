// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "JsonValue.hpp"

namespace ArduinoJson {

class JsonArrayIterator {
  friend class JsonArray;

 public:
  explicit JsonArrayIterator(Internals::JsonNode *node) : _value(node) {}

  JsonValue operator*() const { return _value; }
  JsonValue *operator->() { return &_value; }

  bool operator==(const JsonArrayIterator &other) const {
    return _value._node == other._value._node;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _value._node != other._value._node;
  }

  JsonArrayIterator &operator++() {
    _value._node = _value._node->next;
    return *this;
  }

 private:
  JsonValue _value;
};

class JsonArrayConstIterator {
  friend class JsonArray;

 public:
  explicit JsonArrayConstIterator(Internals::JsonNode *node) : _value(node) {}

  const JsonValue operator*() const { return _value; }
  const JsonValue *operator->() { return &_value; }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _value._node == other._value._node;
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return _value._node != other._value._node;
  }

  JsonArrayConstIterator &operator++() {
    _value._node = _value._node->next;
    return *this;
  }

 private:
  JsonValue _value;
};
}