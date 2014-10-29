// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonObjectConstIterator.hpp"
#include "JsonObjectIterator.hpp"
#include "JsonPrintable.hpp"
#include "Internals/JsonObjectNode.hpp"

namespace ArduinoJson {
class JsonObject : public JsonPrintable {
 public:
  typedef const char *key_type;
  typedef JsonPair value_type;
  typedef JsonObjectIterator iterator;
  typedef JsonObjectConstIterator const_iterator;

  JsonObject(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  int size() const;

  JsonValue &operator[](key_type key);
  void remove(key_type key);

  template <typename T>
  void add(key_type key, T value) {
    (*this)[key] = value;
  }

  JsonArray &createNestedArray(key_type key);
  JsonObject &createNestedObject(key_type key);

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(0); }

  static JsonObject &invalid() { return _invalid; }

  virtual void writeTo(Internals::JsonWriter &writer) const;

 private:
  void addNode(Internals::JsonObjectNode *nodeToAdd);
  void removeNode(Internals::JsonObjectNode *nodeToRemove);

  Internals::JsonObjectNode *getNodeAt(key_type key);
  Internals::JsonObjectNode *getOrCreateNodeAt(key_type key);

  JsonBuffer *_buffer;
  Internals::JsonObjectNode *_firstNode;
  static JsonObject _invalid;
};
}
