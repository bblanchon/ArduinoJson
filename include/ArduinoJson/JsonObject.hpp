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
#include "JsonArray.hpp"

namespace ArduinoJson {
class JsonObject : public JsonPrintable {
  friend class JsonBuffer;

 public:
  typedef const char *key_type;
  typedef JsonPair value_type;
  typedef JsonObjectIterator iterator;
  typedef JsonObjectConstIterator const_iterator;

  int size() const;

  JsonValue &operator[](key_type key);
  void remove(key_type key);

  template <typename T>
  void add(key_type key, T value) {
    add(key).set(value);
  }

  void add(key_type key, JsonArray &array) { add(key).set(array); }
  void add(key_type key, JsonObject &object) { add(key).set(object); }

  JsonArray &createNestedArray(key_type key);
  JsonObject &createNestedObject(key_type key);

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(0); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(0); }

  static JsonObject &invalid() { return _invalid; }

  virtual void writeTo(Internals::JsonWriter &writer) const;

 private:
  // constructor is private, instance must be created via JsonBuffer
  JsonObject(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  JsonObject(const JsonObject &);  // copy is forbidden, use a reference instead
  JsonObject &operator=(
      const JsonObject &);  // copy is forbidden, use a reference instead

  JsonValue &add(key_type key) { return (*this)[key]; }
  void addNode(Internals::JsonObjectNode *nodeToAdd);
  void removeNode(Internals::JsonObjectNode *nodeToRemove);

  Internals::JsonObjectNode *getNodeAt(key_type key);
  Internals::JsonObjectNode *getOrCreateNodeAt(key_type key);

  JsonBuffer *_buffer;
  Internals::JsonObjectNode *_firstNode;
  static JsonObject _invalid;
};
}
