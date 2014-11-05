// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonPrintable.hpp"
#include "Internals/List.hpp"
#include "Internals/ReferenceType.hpp"
#include "JsonPair.hpp"

#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonObject) + (NUMBER_OF_ELEMENTS) * sizeof(JsonObject::node_type))

namespace ArduinoJson {

class JsonArray;
class JsonBuffer;

class JsonObject : public Internals::JsonPrintable<JsonObject>,
                   public Internals::ReferenceType,
                   public Internals::List<JsonPair> {
  friend class JsonBuffer;

 public:
  typedef const char *key_type;
  typedef JsonPair value_type;

  JsonVariant &at(key_type key);
  const JsonVariant &at(key_type key) const;
  JsonVariant &operator[](key_type key);
  const JsonVariant &operator[](key_type key) const { return at(key); }

  void remove(key_type key);

  template <typename T>
  void add(key_type key, T value) {
    add(key).set(value);
  }
  void add(key_type key, JsonArray &array) { add(key).set(array); }
  void add(key_type key, JsonObject &object) { add(key).set(object); }

  JsonArray &createNestedArray(key_type key);
  JsonObject &createNestedObject(key_type key);

  static JsonObject &invalid() { return _invalid; }

  template <typename T>
  void writeTo(T &writer) const;

 private:
  // constructor is private, instance must be created via JsonBuffer
  JsonObject(JsonBuffer *buffer) : List(buffer) {}

  JsonVariant &add(key_type key) { return (*this)[key]; }
  node_type *createNode();
  void addNode(node_type *nodeToAdd);
  void removeNode(node_type *nodeToRemove);

  node_type *getNodeAt(key_type key) const;
  node_type *getOrCreateNodeAt(key_type key);

  static JsonObject _invalid;
};
}
