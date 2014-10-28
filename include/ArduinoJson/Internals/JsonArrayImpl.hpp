// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../ForwardDeclarations.hpp"
#include "JsonArrayIterator.hpp"
#include "JsonArrayConstIterator.hpp"

namespace ArduinoJson {
namespace Internals {
class JsonArrayImpl {
 public:
  typedef JsonValueImpl *value_type;
  typedef JsonArrayIterator iterator;
  typedef JsonArrayConstIterator const_iterator;

  static JsonArrayImpl *createFrom(JsonBuffer *buffer);

  int size() const;

  value_type operator[](int index) const;
  value_type add();

  JsonArrayImpl *createNestedArray();
  JsonObjectImpl *createNestedObject();

  void writeTo(JsonWriter &writer) const;

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(0); }

 private:
  JsonArrayImpl(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  inline void addNode(JsonArrayNode *node);

  JsonBuffer *_buffer;
  Internals::JsonArrayNode *_firstNode;
};
}
}
