// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "String.hpp"
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
  explicit JsonObject(JsonBuffer* buffer) : Internals::List<JsonPair>(buffer) {}

  // Gets or sets the value associated with the specified key.
  JsonObjectSubscript<const char*> operator[](const char* key);
  JsonObjectSubscript<const String&> operator[](const String& key);

  // Gets the value associated with the specified key.
  JsonVariant operator[](JsonObjectKey key) const {
    return get(key);
  }

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
  bool set(
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
  bool set(JsonObjectKey key, const T& value,
           typename TypeTraits::EnableIf<CanSet<T&>::value>::type* = 0) {
    return setNodeAt<T&>(key, const_cast<T&>(value));
  }
  // bool set(Key, float value, uint8_t decimals);
  // bool set(Key, double value, uint8_t decimals);
  template <typename TValue>
  bool set(JsonObjectKey key, TValue value, uint8_t decimals,
           typename TypeTraits::EnableIf<
               TypeTraits::IsFloatingPoint<TValue>::value>::type* = 0) {
    return setNodeAt<const JsonVariant&>(key, JsonVariant(value, decimals));
  }

  // Gets the value associated with the specified key.
  JsonVariant get(JsonObjectKey key) const {
    node_type* node = getNodeAt(key.c_str());
    return node ? node->content.value : JsonVariant();
  }

  // Gets the value associated with the specified key.
  template <typename T>
  T get(JsonObjectKey key) const {
    node_type* node = getNodeAt(key.c_str());
    return node ? node->content.value.as<T>() : JsonVariant::defaultValue<T>();
  }

  // Checks the type of the value associated with the specified key.
  template <typename T>
  bool is(JsonObjectKey key) const {
    node_type* node = getNodeAt(key.c_str());
    return node ? node->content.value.is<T>() : false;
  }

  // Creates and adds a JsonArray.
  // This is a shortcut for JsonBuffer::createArray() and JsonObject::add().
  JsonArray& createNestedArray(JsonObjectKey key);

  // Creates and adds a JsonObject.
  // This is a shortcut for JsonBuffer::createObject() and JsonObject::add().
  JsonObject& createNestedObject(JsonObjectKey key);

  // Tells weither the specified key is present and associated with a value.
  bool containsKey(JsonObjectKey key) const {
    return getNodeAt(key.c_str()) != NULL;
  }

  // Removes the specified key and the associated value.
  void remove(JsonObjectKey key) {
    removeNode(getNodeAt(key.c_str()));
  }

  // Returns a reference an invalid JsonObject.
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonObject& invalid() {
    static JsonObject instance(NULL);
    return instance;
  }

  // Serialize the object to the specified JsonWriter
  void writeTo(Internals::JsonWriter& writer) const {
    writer.beginObject();

    const node_type* node = _firstNode;
    while (node) {
      writer.writeString(node->content.key);
      writer.writeColon();
      node->content.value.writeTo(writer);

      node = node->next;
      if (!node) break;

      writer.writeComma();
    }

    writer.endObject();
  }

 private:
  // Returns the list node that matches the specified key.
  node_type* getNodeAt(const char* key) const {
    for (node_type* node = _firstNode; node; node = node->next) {
      if (!strcmp(node->content.key, key)) return node;
    }
    return NULL;
  }

  template <typename T>
  bool setNodeAt(JsonObjectKey key, T value) {
    node_type* node = getNodeAt(key.c_str());
    if (!node) {
      node = addNewNode();
      if (!node || !setNodeKey(node, key)) return false;
    }
    return setNodeValue<T>(node, value);
  }

  bool setNodeKey(node_type* node, JsonObjectKey key) {
    if (key.needs_copy()) {
      node->content.key = _buffer->strdup(key.c_str());
      if (node->content.key == NULL) return false;
    } else {
      node->content.key = key.c_str();
    }
    return true;
  }

  template <typename T>
  bool setNodeValue(node_type* node, T value) {
    node->content.value = value;
    return true;
  }
};
}
