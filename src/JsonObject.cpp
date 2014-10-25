// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/Internals/JsonNode.hpp"
#include "ArduinoJson/Internals/StringBuilder.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValue JsonObject::operator[](char const *key) {
  JsonNode *node = getOrCreateValueAt(key);
  return JsonValueInternal(node);
}

void JsonObject::remove(char const *key) {
  JsonNode *nodeToRemove = getPairAt(key);
  if (nodeToRemove) removeChild(nodeToRemove);
}

JsonArray JsonObject::createNestedArray(char const *key) {
  JsonNode *node = getOrCreateValueAt(key);

  if (node) node->setAsArray(_node->getContainerBuffer());

  return JsonArray(node);
}

JsonObject JsonObject::createNestedObject(char const *key) {
  JsonNode *node = getOrCreateValueAt(key);

  if (node) node->setAsObject(_node->getContainerBuffer());

  return JsonObject(node);
}

JsonNode *JsonObject::getPairAt(const char *key) {
  for (JsonNode *node = firstChild(); node; node = node->next) {
    if (!strcmp(node->getAsObjectKey(), key)) return node;
  }
  return NULL;
}

JsonNode *JsonObject::getOrCreateValueAt(const char *key) {
  JsonNode *existingNode = getPairAt(key);
  if (existingNode) return existingNode->getAsObjectValue();

  JsonNode *newValueNode = createNode();
  if (!newValueNode) return 0;

  JsonNode *newKeyNode = createNode();
  if (!newKeyNode) return 0;

  newKeyNode->setAsObjectKeyValue(key, newValueNode);

  addChild(newKeyNode);

  return newValueNode;
}
