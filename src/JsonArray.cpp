// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonArray.hpp"

#include "../include/ArduinoJson/Internals/PrettyJsonWriter.hpp"
#include "../include/ArduinoJson/JsonBuffer.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArray JsonArray::_invalid(NULL);

JsonVariant &JsonArray::at(int index) const {
  node_type *node = _firstNode;
  while (node && index--) node = node->next;
  return node ? node->content : JsonVariant::invalid();
}

JsonVariant &JsonArray::add() {
  node_type *node = createNode();
  if (!node) return JsonVariant::invalid();

  addNode(node);

  return node->content;
}

void JsonArray::addNode(node_type *newNode) {
  if (_firstNode) {
    node_type *lastNode = _firstNode;
    while (lastNode->next) lastNode = lastNode->next;
    lastNode->next = newNode;
  } else {
    _firstNode = newNode;
  }
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

template <typename T>
void JsonArray::writeTo(T &writer) const {
  node_type *child = _firstNode;

  if (child) {
    writer.beginArray();

    for (;;) {
      child->content.writeTo(writer);

      child = child->next;
      if (!child) break;

      writer.writeComma();
    }

    writer.endArray();
  } else {
    writer.writeEmptyArray();
  }
}

template void JsonArray::writeTo(JsonWriter &) const;
template void JsonArray::writeTo(PrettyJsonWriter &) const;
