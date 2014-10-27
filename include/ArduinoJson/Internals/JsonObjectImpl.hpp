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

  JsonObjectImpl(JsonBuffer *buffer) : _buffer(buffer) {}

  JsonValueImpl *operator[](const char *key) { return getOrCreateValueAt(key); }
  void remove(key_type key);

  JsonArrayImpl *createNestedArray(key_type key);
  JsonObjectImpl *createNestedObject(key_type key);

  iterator begin() { return iterator(_firstChild); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(_firstChild); }
  const_iterator end() const { return const_iterator(0); }

 private:
  JsonObjectNode *getNodeAt(key_type key);
  void removeNode(JsonObjectNode *nodeToRemove);
  JsonValueImpl *getOrCreateValueAt(key_type key);

  JsonBuffer *_buffer;
  JsonObjectNode *_firstChild;
};
}
}
