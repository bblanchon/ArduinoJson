// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/JsonArrayImpl.hpp"
#include "ArduinoJson/Internals/JsonValueImpl.hpp"
#include "ArduinoJson/Internals/JsonWriter.hpp"
#include "ArduinoJson/Internals/StringBuilder.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

int JsonObjectImpl::size() const {
  int nodeCount = 0;
  for (JsonObjectNode *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

JsonValueImpl *JsonObjectImpl::operator[](const char *key) {
  JsonObjectNode *node = getOrCreateNodeAt(key);
  return node ? &node->value : NULL;
}

void JsonObjectImpl::remove(char const *key) { removeNode(getNodeAt(key)); }

JsonArrayImpl *JsonObjectImpl::createNestedArray(char const *key) {
  JsonObjectNode *node = getOrCreateNodeAt(key);
  if (!node) return NULL;

  JsonArrayImpl *array = new (_buffer) JsonArrayImpl(_buffer);
  node->value.set(array);

  return array;
}

JsonObjectImpl *JsonObjectImpl::createNestedObject(const char *key) {
  JsonObjectNode *node = getOrCreateNodeAt(key);
  if (!node) return NULL;

  JsonObjectImpl *object = new (_buffer) JsonObjectImpl(_buffer);
  node->value.set(object);

  return object;
}

JsonObjectNode *JsonObjectImpl::getNodeAt(const char *key) {
  for (JsonObjectNode *node = _firstNode; node; node = node->next) {
    if (!strcmp(node->key, key)) return node;
  }
  return NULL;
}

JsonObjectNode *JsonObjectImpl::getOrCreateNodeAt(const char *key) {
  JsonObjectNode *existingNode = getNodeAt(key);
  if (existingNode) return existingNode;

  JsonObjectNode *newNode = new (_buffer) JsonObjectNode(key);

  if (newNode) addNode(newNode);

  return newNode;
}

void JsonObjectImpl::addNode(JsonObjectNode *nodeToAdd) {
  if (!_firstNode) {
    _firstNode = nodeToAdd;
  } else {
    JsonObjectNode *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = nodeToAdd;
  }
}

void JsonObjectImpl::removeNode(JsonObjectNode *nodeToRemove) {
  if (nodeToRemove == _firstNode) {
    _firstNode = nodeToRemove->next;
  } else {
    for (JsonObjectNode *node = _firstNode; node; node = node->next)
      if (node->next == nodeToRemove) node->next = nodeToRemove->next;
  }
}

void JsonObjectImpl::writeTo(JsonWriter &writer) const {
  JsonObjectNode *node = _firstNode;

  if (node) {
    writer.beginObject();

    for (;;) {
      writer.writeString(node->key);
      writer.writeColon();
      node->value.writeTo(writer);

      node = node->next;
      if (!node) break;

      writer.writeComma();
    }

    writer.endObject();
  } else {
    writer.writeEmptyObject();
  }
}
