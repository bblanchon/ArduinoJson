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

JsonValue JsonArray::operator[](int index) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if (!index) return *it;
    index--;
  }

  return JsonValue();
}

JsonValue JsonArray::addNewValue() {
  JsonNode *node = createNode();
  if (node) addChild(node);
  return JsonValueInternal(node);
}

JsonArray JsonArray::createNestedArray() {
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
