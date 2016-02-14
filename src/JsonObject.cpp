// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include "../include/ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "../include/ArduinoJson/Internals/StaticStringBuilder.hpp"
#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonBuffer.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonObject JsonObject::_invalid(NULL);

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
