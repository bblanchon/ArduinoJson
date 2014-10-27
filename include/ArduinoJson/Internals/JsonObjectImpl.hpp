// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonObjectConstIterator.hpp"
#include "JsonObjectIterator.hpp"
#include "JsonObjectNode.hpp"

namespace ArduinoJson {
namespace Internals {
class JsonObjectImpl {
 public:
  typedef const char *key_type;
  typedef JsonPair value_type;
  typedef JsonObjectIterator iterator;
  typedef JsonObjectConstIterator const_iterator;

  JsonObjectImpl(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  int size() const;

  JsonValueImpl *operator[](const char *key);
  void remove(key_type key);

  JsonArrayImpl *createNestedArray(key_type key);
  JsonObjectImpl *createNestedObject(key_type key);

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(0); }

  void writeTo(JsonWriter &writer) const;

 private:
  void addNode(JsonObjectNode *nodeToAdd);
  void removeNode(JsonObjectNode *nodeToRemove);

  JsonObjectNode *getNodeAt(key_type key);
  JsonObjectNode *getOrCreateNodeAt(key_type key);

  JsonBuffer *_buffer;
  JsonObjectNode *_firstNode;
};
}
}
