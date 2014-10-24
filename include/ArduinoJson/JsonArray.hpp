// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonContainer.hpp"
#include "JsonArrayIterator.hpp"

namespace ArduinoJson {
class JsonArray : public JsonContainer {
 public:
  typedef JsonArrayIterator iterator;
  typedef JsonArrayConstIterator const_iterator;

  JsonArray() {}

  explicit JsonArray(Internals::JsonNode *node)
      : JsonContainer(node) {}  // TODO: hide

  JsonValue operator[](int index) const;

  void add(bool value);
  void add(const char *value);
  void add(double value, int decimals = 2);
  void add(int value) { add(static_cast<long>(value)); }
  void add(long value);
  void add(JsonContainer nestedArray);  // TODO: should allow JsonValue too

  JsonArray createNestedArray();
  JsonObject createNestedObject();

  bool success() { return _node && _node->isArray(); }

  iterator begin() { return iterator(firstChild()); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(firstChild()); }
  const_iterator end() const { return const_iterator(0); }
};
}
