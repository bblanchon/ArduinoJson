// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonContainer.hpp"
#include "JsonObjectIterator.hpp"

namespace ArduinoJson {
class JsonObject : public JsonContainer {
 public:
  JsonObject() {}

  explicit JsonObject(Internals::JsonNode *node) : JsonContainer(node) {}

  JsonValue operator[](const char *key);
  void remove(const char *key);

  JsonArray createNestedArray(const char *key);
  JsonObject createNestedObject(const char *key);

  bool success() { return _node && _node->isObject(); }

  JsonObjectIterator begin();

  JsonObjectIterator end() { return JsonObjectIterator(0); }

 private:
  Internals::JsonNode *getOrCreateNodeAt(const char *key);
};
}
