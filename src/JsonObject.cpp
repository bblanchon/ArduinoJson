// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonObject.hpp"

#include <new>       // required for placement new
#include <string.h>  // for strcmp

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/Internals/JsonWriter.hpp"
#include "ArduinoJson/Internals/StringBuilder.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonObject JsonObject::_invalid(NULL);

int JsonObject::size() const {
  int nodeCount = 0;
  for (JsonObjectNode *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

JsonValue &JsonObject::at(const char *key) {
  JsonObjectNode *node = getOrCreateNodeAt(key);
  return node ? node->pair.value : JsonValue::invalid();
}

void JsonObject::remove(char const *key) { removeNode(getNodeAt(key)); }

JsonArray &JsonObject::createNestedArray(char const *key) {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  add(key, array);
  return array;
}

JsonObject &JsonObject::createNestedObject(const char *key) {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &object = _buffer->createObject();
  add(key, object);
  return object;
}

JsonObjectNode *JsonObject::getNodeAt(const char *key) {
  for (JsonObjectNode *node = _firstNode; node; node = node->next) {
    if (!strcmp(node->pair.key, key)) return node;
  }
  return NULL;
}

JsonObjectNode *JsonObject::getOrCreateNodeAt(const char *key) {
  JsonObjectNode *existingNode = getNodeAt(key);
  if (existingNode) return existingNode;

  JsonObjectNode *newNode = createNode(key);

  if (newNode) addNode(newNode);

  return newNode;
}

JsonObjectNode *JsonObject::createNode(const char *key) {
  if (!_buffer) return NULL;
  void *ptr = _buffer->alloc(sizeof(JsonObjectNode));
  return ptr ? new (ptr) JsonObjectNode(key) : NULL;
}

void JsonObject::addNode(JsonObjectNode *nodeToAdd) {
  if (!_firstNode) {
    _firstNode = nodeToAdd;
  } else {
    JsonObjectNode *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = nodeToAdd;
  }
}

void JsonObject::removeNode(JsonObjectNode *nodeToRemove) {
  if (nodeToRemove == _firstNode) {
    _firstNode = nodeToRemove->next;
  } else {
    for (JsonObjectNode *node = _firstNode; node; node = node->next)
      if (node->next == nodeToRemove) node->next = nodeToRemove->next;
  }
}

void JsonObject::writeTo(JsonWriter &writer) const {
  JsonObjectNode *node = _firstNode;

  if (node) {
    writer.beginObject();

    for (;;) {
      writer.writeString(node->pair.key);
      writer.writeColon();
      node->pair.value.writeTo(writer);

      node = node->next;
      if (!node) break;

      writer.writeComma();
    }

    writer.endObject();
  } else {
    writer.writeEmptyObject();
  }
}
