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

JsonVariant &JsonObject::at(const char *key) {
  node_type *node = getNodeAt(key);
  return node ? node->content.value : JsonVariant::invalid();
}

const JsonVariant &JsonObject::at(const char *key) const {
  node_type *node = getNodeAt(key);
  return node ? node->content.value : JsonVariant::invalid();
}

JsonVariant &JsonObject::operator[](const char *key) {
  node_type *node = getOrCreateNodeAt(key);
  return node ? node->content.value : JsonVariant::invalid();
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

JsonObject::node_type *JsonObject::getNodeAt(const char *key) const {
  for (node_type *node = _firstNode; node; node = node->next) {
    if (!strcmp(node->content.key, key)) return node;
  }
  return NULL;
}

JsonObject::node_type *JsonObject::getOrCreateNodeAt(const char *key) {
  node_type *existingNode = getNodeAt(key);
  if (existingNode) return existingNode;

  node_type *newNode = createNode();
  if (!newNode) return NULL;

  newNode->content.key = key;
  addNode(newNode);
  return newNode;
}

JsonObject::node_type *JsonObject::createNode() {
  if (!_buffer) return NULL;
  void *ptr = _buffer->alloc(sizeof(node_type));
  return ptr ? new (ptr) node_type() : NULL;
}

void JsonObject::addNode(node_type *nodeToAdd) {
  if (!_firstNode) {
    _firstNode = nodeToAdd;
  } else {
    node_type *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = nodeToAdd;
  }
}

void JsonObject::removeNode(node_type *nodeToRemove) {
  if (!nodeToRemove) return;
  if (nodeToRemove == _firstNode) {
    _firstNode = nodeToRemove->next;
  } else {
    for (node_type *node = _firstNode; node; node = node->next)
      if (node->next == nodeToRemove) node->next = nodeToRemove->next;
  }
}

template <typename T>
void JsonObject::writeTo(T &writer) const {
  node_type *node = _firstNode;

  if (node) {
    writer.beginObject();

    for (;;) {
      writer.writeString(node->content.key);
      writer.writeColon();
      node->content.value.writeTo(writer);

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
