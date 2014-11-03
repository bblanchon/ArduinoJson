// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonArray.hpp"

#include "../include/ArduinoJson/Internals/PlacementNew.hpp"
#include "../include/ArduinoJson/Internals/PrettyJsonWriter.hpp"
#include "../include/ArduinoJson/JsonBuffer.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArray JsonArray::_invalid(NULL);

int JsonArray::size() const {
  int nodeCount = 0;
  for (JsonArrayNode *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

JsonValue &JsonArray::at(int index) const {
  JsonArrayNode *node = _firstNode;
  while (node && index--) node = node->next;
  return node ? node->value : JsonValue::invalid();
}

JsonValue &JsonArray::add() {
  JsonArrayNode *node = createNode();
  if (!node) return JsonValue::invalid();

  addNode(node);

  return node->value;
}

JsonArrayNode *JsonArray::createNode() {
  if (!_buffer) return NULL;
  void *ptr = _buffer->alloc(sizeof(JsonArrayNode));
  return ptr ? new (ptr) JsonArrayNode() : NULL;
}

void JsonArray::addNode(JsonArrayNode *newNode) {
  if (_firstNode) {
    JsonArrayNode *lastNode = _firstNode;
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
  JsonArrayNode *child = _firstNode;

  if (child) {
    writer.beginArray();

    for (;;) {
      child->value.writeTo(writer);

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
