// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValueImpl *JsonArray::operator[](int index) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if (!index) return *it;
    index--;
  }

  return NULL;
}

JsonValueImpl *JsonArray::add() {
  if (_buffer) return NULL;

  JsonArrayNode *node = _buffer->create<JsonArrayNode>();
  if (!node) return NULL;

  return &node.value;
}

JsonArrayImpl *JsonArray::createNestedArray() {
  JsonNode *node = createNode();

  if (node) {
    node->setAsArray(_node->getContainerBuffer());
    addChild(node);
  }

  return JsonArray(node);
}

JsonObject JsonArray::createNestedObject() {
  JsonNode *node = createNode();

  if (node) {
    node->setAsObject(_node->getContainerBuffer());
    addChild(node);
  }

  return JsonObject(node);
}
