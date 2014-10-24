// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "JsonValue.hpp"

namespace ArduinoJson {

template <typename T>
class JsonIterator {
  friend class JsonArray;

 public:
  explicit JsonIterator(Internals::JsonNode *node) : _value(node) {}

  T operator*() const { return _value; }
  T *operator->() { return &_value; }

  bool operator==(const JsonIterator &other) const {
    return _value._node == other._value._node;
  }

  bool operator!=(const JsonIterator &other) const {
    return _value._node != other._value._node;
  }

  JsonIterator &operator++() {
    _value._node = _value._node->next;
    return *this;
  }

 private:
  T _value;
};

template <typename T>
class JsonConstIterator {
  friend class JsonArray;

 public:
  explicit JsonConstIterator(Internals::JsonNode *node) : _value(node) {}

  const JsonValue operator*() const { return _value; }
  const JsonValue *operator->() { return &_value; }

  bool operator==(const JsonConstIterator &other) const {
    return _value._node == other._value._node;
  }

  bool operator!=(const JsonConstIterator &other) const {
    return _value._node != other._value._node;
  }

  JsonConstIterator &operator++() {
    _value._node = _value._node->next;
    return *this;
  }

 private:
  JsonValue _value;
};
}