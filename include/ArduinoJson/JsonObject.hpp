// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Arduino/String.hpp"
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
  // Create an empty JsonArray attached to the specified JsonBuffer.
  // You should not use this constructor directly.
  // Instead, use JsonBuffer::createObject() or JsonBuffer.parseObject().
  FORCE_INLINE explicit JsonObject(JsonBuffer* buffer)
      : Internals::List<JsonPair>(buffer) {}

  // Gets or sets the value associated with the specified key.
  FORCE_INLINE JsonObjectSubscript<const char*> operator[](const char* key);
  FORCE_INLINE JsonObjectSubscript<const String&> operator[](const String& key);

  // Gets the value associated with the specified key.
  FORCE_INLINE JsonVariant operator[](JsonObjectKey key) const;

  // Sets the specified key with the specified value.
  FORCE_INLINE bool set(const char* key, bool value);
  FORCE_INLINE bool set(const char* key, float value, uint8_t decimals = 2);
  FORCE_INLINE bool set(const char* key, double value, uint8_t decimals = 2);
  FORCE_INLINE bool set(const char* key, signed char value);
  FORCE_INLINE bool set(const char* key, signed long value);
  FORCE_INLINE bool set(const char* key, signed int value);
  FORCE_INLINE bool set(const char* key, signed short value);
  FORCE_INLINE bool set(const char* key, unsigned char value);
  FORCE_INLINE bool set(const char* key, unsigned long value);
  FORCE_INLINE bool set(const char* key, unsigned int value);
  FORCE_INLINE bool set(const char* key, unsigned short value);
  FORCE_INLINE bool set(const char* key, const char* value);
  FORCE_INLINE bool set(const char* key, const String& value);
  FORCE_INLINE bool set(const char* key, JsonArray& array);
  FORCE_INLINE bool set(const char* key, JsonObject& object);
  FORCE_INLINE bool set(const char* key, const JsonVariant& value);
  template <typename T>
  FORCE_INLINE bool set(const char* key, const T& value);

  FORCE_INLINE bool set(const String& key, bool value);
  FORCE_INLINE bool set(const String& key, float value, uint8_t decimals = 2);
  FORCE_INLINE bool set(const String& key, double value, uint8_t decimals = 2);
  FORCE_INLINE bool set(const String& key, signed char value);
  FORCE_INLINE bool set(const String& key, signed long value);
  FORCE_INLINE bool set(const String& key, signed int value);
  FORCE_INLINE bool set(const String& key, signed short value);
  FORCE_INLINE bool set(const String& key, unsigned char value);
  FORCE_INLINE bool set(const String& key, unsigned long value);
  FORCE_INLINE bool set(const String& key, unsigned int value);
  FORCE_INLINE bool set(const String& key, unsigned short value);
  FORCE_INLINE bool set(const String& key, const char* value);
  FORCE_INLINE bool set(const String& key, const String& value);
  FORCE_INLINE bool set(const String& key, JsonArray& array);
  FORCE_INLINE bool set(const String& key, JsonObject& object);
  FORCE_INLINE bool set(const String& key, const JsonVariant& value);
  template <typename T>
  FORCE_INLINE bool set(const String& key, const T& value);

  // Gets the value associated with the specified key.
  FORCE_INLINE JsonVariant get(JsonObjectKey) const;

  // Gets the value associated with the specified key.
  template <typename T>
  FORCE_INLINE T get(JsonObjectKey) const;

  // Checks the type of the value associated with the specified key.
  template <typename T>
  FORCE_INLINE bool is(JsonObjectKey) const;

  // Creates and adds a JsonArray.
  // This is a shortcut for JsonBuffer::createArray() and JsonObject::add().
  FORCE_INLINE JsonArray& createNestedArray(const char* key);
  FORCE_INLINE JsonArray& createNestedArray(const String& key);

  // Creates and adds a JsonObject.
  // This is a shortcut for JsonBuffer::createObject() and JsonObject::add().
  FORCE_INLINE JsonObject& createNestedObject(const char* key);
  FORCE_INLINE JsonObject& createNestedObject(const String& key);

  // Tells weither the specified key is present and associated with a value.
  FORCE_INLINE bool containsKey(JsonObjectKey key) const;

  // Removes the specified key and the associated value.
  void remove(JsonObjectKey key);

  // Returns a reference an invalid JsonObject.
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonObject& invalid() { return _invalid; }

  // Serialize the object to the specified JsonWriter
  void writeTo(Internals::JsonWriter& writer) const;

 private:
  // Returns the list node that matches the specified key.
  node_type* getNodeAt(JsonObjectKey key) const;

  node_type* getOrCreateNodeAt(JsonObjectKey key);

  template <typename TKey, typename TValue>
  FORCE_INLINE bool setNodeAt(TKey key, TValue value);

  template <typename TKey>
  JsonArray& createArrayAt(TKey key);

  template <typename TKey>
  JsonObject& createObjectAt(TKey key);

  template <typename T>
  FORCE_INLINE void setNodeKey(node_type*, T key);

  template <typename T>
  FORCE_INLINE void setNodeValue(node_type*, T value);

  // The instance returned by JsonObject::invalid()
  static JsonObject _invalid;
};
}

#include "JsonObject.ipp"
