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

// Forward declarations
class JsonObject;
class JsonBuffer;

// An array of JsonVariant.
//
// The constructor is private, instances must be created via
// JsonBuffer::createArray() or JsonBuffer::parseArray().
// A JsonArray can be serialized to a JSON string via JsonArray::printTo().
// It can also be deserialized from a JSON string via JsonBuffer::parseArray().
class JsonArray : public Internals::JsonPrintable<JsonArray>,
                  public Internals::ReferenceType,
                  public Internals::List<JsonVariant> {
  // JsonBuffer is a friend because it needs to call the private constructor.
  friend class JsonBuffer;

 public:
  // Returns the JsonVariant at the specified index (synonym for operator[])
  JsonVariant &at(int index) const;

  // Returns the JsonVariant at the specified index (synonym for at())
  JsonVariant &operator[](int index) const { return at(index); }

  // Adds an uninitialized JsonVariant at the end of the array.
  // Return a reference or JsonVariant::invalid() if allocation fails.
  JsonVariant &add();

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
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonArray &invalid() { return _invalid; }

  // Serialize the array to the specified JsonWriter.
  void writeTo(Internals::JsonWriter &writer) const;

 private:
  // Create an empty JsonArray attached to the specified JsonBuffer.
  explicit JsonArray(JsonBuffer *buffer)
      : Internals::List<JsonVariant>(buffer) {}

  // The instance returned by JsonArray::invalid()
  static JsonArray _invalid;
};
}
