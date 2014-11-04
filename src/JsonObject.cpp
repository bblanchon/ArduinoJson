// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "../include/ArduinoJson/Internals/PlacementNew.hpp"
#include "../include/ArduinoJson/Internals/PrettyJsonWriter.hpp"
#include "../include/ArduinoJson/Internals/StringBuilder.hpp"
#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonBuffer.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonObject JsonObject::_invalid(NULL);

int JsonObject::size() const {
  int nodeCount = 0;
  for (JsonObjectNode *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

JsonVariant &JsonObject::at(const char *key) {
  JsonObjectNode *node = getNodeAt(key);
  return node ? node->pair.value : JsonVariant::invalid();
}

const JsonVariant &JsonObject::at(const char *key) const {
  JsonObjectNode *node = getNodeAt(key);
  return node ? node->pair.value : JsonVariant::invalid();
}

JsonVariant &JsonObject::operator[](const char *key) {
  JsonObjectNode *node = getOrCreateNodeAt(key);
  return node ? node->pair.value : JsonVariant::invalid();
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

JsonObjectNode *JsonObject::getNodeAt(const char *key) const {
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
  if (!nodeToRemove) return;
  if (nodeToRemove == _firstNode) {
    _firstNode = nodeToRemove->next;
  } else {
    for (JsonObjectNode *node = _firstNode; node; node = node->next)
      if (node->next == nodeToRemove) node->next = nodeToRemove->next;
  }
}

template <typename T>
void JsonObject::writeTo(T &writer) const {
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

template void JsonObject::writeTo(JsonWriter &writer) const;
template void JsonObject::writeTo(PrettyJsonWriter &writer) const;
