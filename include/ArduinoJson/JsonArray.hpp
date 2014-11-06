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

// Returns the size (in bytes) of an array with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonArray) + (NUMBER_OF_ELEMENTS) * sizeof(JsonArray::node_type))

namespace ArduinoJson {

// Forward declaration
class JsonObject;
class JsonBuffer;

// An array of JsonVariant.
// Can be converted to a JSON string via JsonArray::printTo().
// Can be extracted from a JSON string JsonBuffer::parseArray().
class JsonArray : public Internals::JsonPrintable<JsonArray>,
                  public Internals::ReferenceType,
                  public Internals::List<JsonVariant> {
  // JsonBuffer is a friend because it needs to call the private constructor of
  // JsonArray from createArray() and parseArray().
  friend class JsonBuffer;

 public:
  // Returns the JsonVariant at the specified index (synonym for at())
  value_type &operator[](int index) const { return at(index); }

  // Returns the JsonVariant at the specified index (synonym for operator[])
  value_type &at(int index) const;

  // Adds an uninitialized JsonVariant at the end of the array.
  value_type &add();

  // Adds the specified value at the end of the array.
  template <typename T>
  void add(T value) {
    add().set(value);
  }

  // Adds the specified double value at the end of the array.
  // The value will be printed with the specified number of decimal digits.
  void add(double value, uint8_t decimals) { add().set(value, decimals); }

  // Adds a reference to the specified JsonArray at the end of the array.
  void add(JsonArray &array) { add().set(array); }

  // Adds a reference to the specified JsonObject at the end of the array.
  void add(JsonObject &obejct) { add().set(obejct); }

  // Creates a JsonArray and adds a reference at the end of the array.
  // It's a shortcut for JsonBuffer::createArray() and JsonArray::add()
  JsonArray &createNestedArray();

  // Creates a JsonObject and adds a reference at the end of the array.
  // It's a shortcut for JsonBuffer::createObject() and JsonArray::add()
  JsonObject &createNestedObject();

  // Returns a reference an invalid JsonArray.
  // This is used when memory allocation or JSON parsing fail.
  static JsonArray &invalid() { return _invalid; }

  // Writes the array to a JsonWriter.
  template <typename T>
  void writeTo(T &writer) const;

 private:
  // Create an empty JsonArray attached to the specified JsonBuffer.
  // Constructor is private: instance must be created via a JsonBuffer
  explicit JsonArray(JsonBuffer *buffer) : List(buffer) {}

  // The instance returned by JsonArray::invalid()
  static JsonArray _invalid;
};
}
