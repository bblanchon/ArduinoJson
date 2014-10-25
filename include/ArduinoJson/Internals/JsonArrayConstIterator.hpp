// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonValueInternal.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonArrayConstIterator {
 public:
  explicit JsonArrayConstIterator(Internals::JsonNode *node) : _value(node) {}

  JsonValue operator*() const { return _value; }
  JsonValue *operator->() { return &_value; }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _value.isSameAs(other._value);
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return !_value.isSameAs(other._value);
  }

  JsonArrayConstIterator &operator++() {
    _value.moveToNext();
    return *this;
  }

 private:
  JsonValueInternal _value;
};
}
}
