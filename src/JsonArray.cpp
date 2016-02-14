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

void JsonArray::removeAt(size_t index) { removeNode(getNodeAt(index)); }

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
