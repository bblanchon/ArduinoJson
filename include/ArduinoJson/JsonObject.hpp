// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Arduino/String.hpp"
#include "Internals/JsonBufferAllocated.hpp"
#include "Internals/JsonPrintable.hpp"
#include "Internals/List.hpp"
#include "Internals/ReferenceType.hpp"
#include "JsonPair.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsFloatingPoint.hpp"
#include "TypeTraits/IsReference.hpp"
#include "TypeTraits/IsSame.hpp"

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
  // A meta-function that returns true if type T can be used in
  // JsonObject::set()
  template <typename T>
  struct CanSet {
    static const bool value = JsonVariant::IsConstructibleFrom<T>::value ||
                              TypeTraits::IsSame<T, String&>::value ||
                              TypeTraits::IsSame<T, const String&>::value;
  };

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
  // bool set(TKey key, bool value);
  // bool set(TKey key, char value);
  // bool set(TKey key, long value);
  // bool set(TKey key, int value);
  // bool set(TKey key, short value);
  // bool set(TKey key, float value);
  // bool set(TKey key, double value);
  // bool set(TKey key, const char* value);
  // bool set(TKey key, RawJson value);
  template <typename T>
  FORCE_INLINE bool set(
      JsonObjectKey key, T value,
      typename TypeTraits::EnableIf<
          CanSet<T>::value && !TypeTraits::IsReference<T>::value>::type* = 0) {
    return setNodeAt<T>(key, value);
  }
  // bool set(Key, String&);
  // bool set(Key, JsonArray&);
  // bool set(Key, JsonObject&);
  // bool set(Key, JsonVariant&);
  template <typename T>
  FORCE_INLINE bool set(
      JsonObjectKey key, const T& value,
      typename TypeTraits::EnableIf<CanSet<T&>::value>::type* = 0) {
    return setNodeAt<T&>(key, const_cast<T&>(value));
  }
  // bool set(Key, float value, uint8_t decimals);
  // bool set(Key, double value, uint8_t decimals);
  template <typename TValue>
  FORCE_INLINE bool set(
      JsonObjectKey key, TValue value, uint8_t decimals,
      typename TypeTraits::EnableIf<
          TypeTraits::IsFloatingPoint<TValue>::value>::type* = 0) {
    return setNodeAt<const JsonVariant&>(key, JsonVariant(value, decimals));
  }

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
  FORCE_INLINE JsonArray& createNestedArray(JsonObjectKey key);

  // Creates and adds a JsonObject.
  // This is a shortcut for JsonBuffer::createObject() and JsonObject::add().
  FORCE_INLINE JsonObject& createNestedObject(JsonObjectKey key);

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
  node_type* getNodeAt(const char* key) const;

  node_type* getOrCreateNodeAt(const char* key);

  template <typename T>
  FORCE_INLINE bool setNodeAt(JsonObjectKey key, T value);

  FORCE_INLINE bool setNodeKey(node_type*, JsonObjectKey key);

  template <typename T>
  FORCE_INLINE bool setNodeValue(node_type*, T value);

  // The instance returned by JsonObject::invalid()
  static JsonObject _invalid;
};
}

#include "JsonObject.ipp"
