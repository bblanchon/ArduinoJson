// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/ReferenceType.hpp"
#include "JsonObjectConstIterator.hpp"
#include "JsonObjectIterator.hpp"
#include "JsonPrintable.hpp"

#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonObject) +                      \
   (NUMBER_OF_ELEMENTS) * sizeof(Internals::JsonObjectNode))

namespace ArduinoJson {

class JsonArray;
class JsonBuffer;

class JsonObject : public JsonPrintable<JsonObject>,
                   public Internals::ReferenceType {
  friend class JsonBuffer;

 public:
  typedef const char *key_type;
  typedef JsonPair value_type;
  typedef JsonObjectIterator iterator;
  typedef JsonObjectConstIterator const_iterator;

  bool success() const { return _buffer != NULL; }
  int size() const;

  JsonValue &at(key_type key);
  const JsonValue &at(key_type key) const;
  JsonValue &operator[](key_type key);
  const JsonValue &operator[](key_type key) const { return at(key); }

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
  iterator end() { return iterator(NULL); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(NULL); }

  static JsonObject &invalid() { return _invalid; }

  template <typename T>
  void writeTo(T &writer) const;

 private:
  // constructor is private, instance must be created via JsonBuffer
  JsonObject(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  JsonValue &add(key_type key) { return (*this)[key]; }
  Internals::JsonObjectNode *createNode(key_type key);
  void addNode(Internals::JsonObjectNode *nodeToAdd);
  void removeNode(Internals::JsonObjectNode *nodeToRemove);

  Internals::JsonObjectNode *getNodeAt(key_type key) const;
  Internals::JsonObjectNode *getOrCreateNodeAt(key_type key);

  JsonBuffer *_buffer;
  Internals::JsonObjectNode *_firstNode;
  static JsonObject _invalid;
};
}
