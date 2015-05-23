// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/JsonBufferAllocated.hpp"
#include "Internals/JsonPrintable.hpp"
#include "Internals/List.hpp"
#include "Internals/ReferenceType.hpp"
#include "JsonPair.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonObject) + (NUMBER_OF_ELEMENTS) * sizeof(JsonObject::node_type))

namespace ArduinoJson {

// Forward declarations
class JsonArray;
class JsonBuffer;
class JsonObjectSubscript;

// A dictionary of JsonVariant indexed by string (char*)
//
// The constructor is private, instances must be created via
// JsonBuffer::createObject() or JsonBuffer::parseObject().
// A JsonObject can be serialized to a JSON string via JsonObject::printTo().
// It can also be deserialized from a JSON string via JsonBuffer::parseObject().
class JsonObject : public Internals::JsonPrintable<JsonObject>,
                   public Internals::ReferenceType,
                   public Internals::List<JsonPair>,
                   public Internals::JsonBufferAllocated {
 public:
  typedef const char *key_type;
  typedef JsonPair value_type;

  // Create an empty JsonArray attached to the specified JsonBuffer.
  // You should not use this constructor directly.
  // Instead, use JsonBuffer::createObject() or JsonBuffer.parseObject().
  JSON_FORCE_INLINE explicit JsonObject(JsonBuffer *buffer)
      : Internals::List<JsonPair>(buffer) {}

  // Gets or sets the value associated with the specified key.
  JSON_FORCE_INLINE JsonObjectSubscript operator[](key_type key);

  // Gets the value associated with the specified key.
  JSON_FORCE_INLINE const JsonObjectSubscript operator[](key_type key) const;

  // Sets the specified key with the specified value.
  JSON_FORCE_INLINE bool set(key_type key, const JsonVariant value);

  // Gets the value associated with the specified key.
  JSON_FORCE_INLINE JsonVariant get(key_type key) const;

  // Gets the value associated with the specified key.
  template <typename T>
  JSON_FORCE_INLINE T get(key_type key) const;

  // Checks the type of the value associated with the specified key.
  template <typename T>
  JSON_FORCE_INLINE T is(key_type key) const;

  // Creates and adds a JsonArray.
  // This is a shortcut for JsonBuffer::createArray() and JsonObject::add().
  JsonArray &createNestedArray(key_type key);

  // Creates and adds a JsonObject.
  // This is a shortcut for JsonBuffer::createObject() and JsonObject::add().
  JsonObject &createNestedObject(key_type key);

  // Tells weither the specified key is present and associated with a value.
  JSON_FORCE_INLINE bool containsKey(key_type key) const;

  // Removes the specified key and the associated value.
  void remove(key_type key);

  // Returns a reference an invalid JsonObject.
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonObject &invalid() { return _invalid; }

  // Serialize the object to the specified JsonWriter
  void writeTo(Internals::JsonWriter &writer) const;

 private:
  // Returns the list node that matches the specified key.
  node_type *getNodeAt(key_type key) const;

  node_type *getOrCreateNodeAt(const char *key);

  // The instance returned by JsonObject::invalid()
  static JsonObject _invalid;
};
}

#include "JsonObject.ipp"
