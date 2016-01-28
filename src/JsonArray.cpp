// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../include/ArduinoJson/JsonArray.hpp"

#include "../include/ArduinoJson/JsonBuffer.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArray JsonArray::_invalid(NULL);

JsonArray::node_type *JsonArray::getNodeAt(size_t index) const {
  node_type *node = _firstNode;
  while (node && index--) node = node->next;
  return node;
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

void JsonArray::removeAt(size_t index) { removeNode(getNodeAt(index)); }

size_t JsonArray::writeTo(JsonWriter &writer) const {
  size_t written = 0;
  if((written = writer.beginArray()) == 0)
    return 0;

  const node_type *child = _firstNode;
  size_t total = written;
  while (child) {
    if((written = child->content.writeTo(writer)) == 0) break;
    total += written;

    child = child->next;
    if (!child) break;

    if((written = writer.writeComma()) == 0) break;
    total += written;
  }

  if(written)
    total += writer.endArray();
  return total;
}
