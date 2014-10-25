// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonObjectConstIterator.hpp"
#include "Internals/JsonObjectIterator.hpp"
#include "JsonContainer.hpp"

namespace ArduinoJson {
class JsonObject : public JsonContainer {
 public:
  typedef JsonPair value_type;
  typedef Internals::JsonObjectIterator iterator;
  typedef Internals::JsonObjectConstIterator const_iterator;

  JsonObject() {}

  explicit JsonObject(Internals::JsonNode *node) : JsonContainer(node) {}

  JsonValue operator[](const char *key);
  void remove(const char *key);

  JsonArray createNestedArray(const char *key);
  JsonObject createNestedObject(const char *key);

  bool success() { return _node && _node->isObject(); }

  iterator begin() { return iterator(firstChild()); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(firstChild()); }
  const_iterator end() const { return const_iterator(0); }

 private:
  Internals::JsonNode *getPairAt(const char *key);
  Internals::JsonNode *getOrCreateValueAt(const char *key);
};
}
