// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonPrintable.hpp"
#include "Internals/List.hpp"
#include "Internals/ReferenceType.hpp"
#include "JsonVariant.hpp"

#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonArray) + (NUMBER_OF_ELEMENTS) * sizeof(JsonArray::node_type))

namespace ArduinoJson {

class JsonObject;
class JsonBuffer;

class JsonArray : public Internals::JsonPrintable<JsonArray>,
                  public Internals::ReferenceType,
                  public Internals::List<JsonVariant> {
  friend class JsonBuffer;

 public:
  value_type &operator[](int index) const { return at(index); }
  value_type &at(int index) const;

  template <typename T>
  void add(T value) {
    add().set(value);
  }

  value_type &add();
  void add(double value, uint8_t decimals) { add().set(value, decimals); }
  void add(JsonArray &nestedArray) { add().set(nestedArray); }
  void add(JsonObject &nestedObject) { add().set(nestedObject); }

  JsonArray &createNestedArray();
  JsonObject &createNestedObject();

  static JsonArray &invalid() { return _invalid; }

  template <typename T>
  void writeTo(T &writer) const;

 private:
  // constructor is private: instance must be created via a JsonBuffer
  explicit JsonArray(JsonBuffer *buffer) : List(buffer) {}

  static JsonArray _invalid;
};
}
