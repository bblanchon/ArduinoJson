// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonArrayConstIterator.hpp"
#include "Internals/JsonArrayIterator.hpp"
#include "JsonContainer.hpp"

namespace ArduinoJson {
class JsonArray : public JsonContainer {
 public:
  typedef JsonValue value_type;
  typedef Internals::JsonArrayIterator iterator;
  typedef Internals::JsonArrayConstIterator const_iterator;

  JsonArray() {}

  explicit JsonArray(Internals::JsonNode *node)
      : JsonContainer(node) {}  // TODO: hide

  JsonValue operator[](int index) const;

  template <typename T>
  void add(T value) {
    addNewValue() = value;
  }

  void add(double value, int decimals = 2) {
    addNewValue().set(value, decimals);
  }

  JsonArray createNestedArray();
  JsonObject createNestedObject();

  bool success() { return _node && _node->isArray(); }

  iterator begin() { return iterator(firstChild()); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(firstChild()); }
  const_iterator end() const { return const_iterator(0); }

private:
  JsonValue addNewValue();
};
}
