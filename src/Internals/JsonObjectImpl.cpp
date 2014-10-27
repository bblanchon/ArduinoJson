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
#include "ArduinoJson/Internals/StringBuilder.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

void JsonObjectImpl::remove(char const *key) { removeNode(getNodeAt(key)); }

JsonArrayImpl *JsonObjectImpl::createNestedArray(char const *key) {
  JsonValueImpl *node = getOrCreateValueAt(key);
  if (!node) return NULL;

  JsonArrayImpl *array = new (_buffer) JsonArrayImpl(_buffer);
  node->set(array);

  return array;
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

void JsonNode::addChild(JsonNode *childToAdd) {
  if (type == JSON_PROXY) return content.asProxy.target->addChild(childToAdd);

  if (type != JSON_ARRAY && type != JSON_OBJECT) return;

  JsonNode *lastChild = content.asContainer.child;

  if (!lastChild) {
    content.asContainer.child = childToAdd;
    return;
  }

  while (lastChild->next) lastChild = lastChild->next;

  lastChild->next = childToAdd;
}

void JsonNode::removeChild(JsonNode *childToRemove) {
  if (type == JSON_PROXY)
    return content.asProxy.target->removeChild(childToRemove);

  if (type != JSON_ARRAY && type != JSON_OBJECT) return;

  if (content.asContainer.child == childToRemove) {
    content.asContainer.child = childToRemove->next;
    return;
  }

  for (JsonNode *child = content.asContainer.child; child;
       child = child->next) {
    if (child->next == childToRemove) child->next = childToRemove->next;
  }
}

void JsonObjectImpl::writeObjectTo(JsonWriter &writer) {
  JsonObjectNode *child = _firstChild;

  if (child) {
    writer.beginObject();

    for (;;) {
      writer.writeString(child->content.asKeyValue.key);
      writer.writeColon();
      child->value->writeTo(writer);

      child = child->next;
      if (!child) break;

      writer.writeComma();
    }

    writer.endObject();
  } else {
    writer.writeEmptyObject();
  }
}
