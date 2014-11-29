// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "../include/ArduinoJson/Internals/PlacementNew.hpp"
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
  // try to find an existing node
  node_type *node = getNodeAt(key);

  // not fount => create a new one
  if (!node) {
    node = createNode();
    if (!node) return JsonVariant::invalid();

    node->content.key = key;
    addNode(node);
  }

  return node->content.value;
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

void JsonObject::writeTo(JsonWriter &writer) const {
  writer.beginObject();

  const node_type *node = _firstNode;
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
