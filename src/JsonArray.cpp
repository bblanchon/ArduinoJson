// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonArray.hpp"

#include "../include/ArduinoJson/JsonBuffer.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArray JsonArray::_invalid(NULL);

JsonVariant &JsonArray::at(int index) const {
  node_type *node = getNodeAt(index);
  return node ? node->content : JsonVariant::invalid();
}

JsonVariant &JsonArray::add() {
  node_type *node = createNode();
  if (!node) return JsonVariant::invalid();

  addNode(node);

  return node->content;
}

JsonArray &JsonArray::createNestedArray() {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  add(array);
  return array;
}

JsonObject &JsonArray::createNestedObject() {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &object = _buffer->createObject();
  add(object);
  return object;
}

JsonArray::node_type *JsonArray::getNodeAt(int index) const {
  node_type *node = _firstNode;
  while (node && index--) node = node->next;
  return node;
}

void JsonArray::removeAt(int index) { removeNode(getNodeAt(index)); }

void JsonArray::writeTo(JsonWriter &writer) const {
  writer.beginArray();

  const node_type *child = _firstNode;
  while (child) {
    child->content.writeTo(writer);

    child = child->next;
    if (!child) break;

    writer.writeComma();
  }

  writer.endArray();
}
