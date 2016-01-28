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

JsonObject::node_type *JsonObject::getOrCreateNodeAt(JsonObjectKey key) {
  node_type *existingNode = getNodeAt(key);
  if (existingNode) return existingNode;

  node_type *newNode = addNewNode();
  return newNode;
}

template <typename TKey>
JsonArray &JsonObject::createArrayAt(TKey key) {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  setNodeAt<TKey, const JsonVariant &>(key, array);
  return array;
}
template JsonArray &JsonObject::createArrayAt<const char *>(const char *);
template JsonArray &JsonObject::createArrayAt<const String &>(const String &);

template <typename TKey>
JsonObject &JsonObject::createObjectAt(TKey key) {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &array = _buffer->createObject();
  setNodeAt<TKey, const JsonVariant &>(key, array);
  return array;
}
template JsonObject &JsonObject::createObjectAt<const char *>(const char *);
template JsonObject &JsonObject::createObjectAt<const String &>(const String &);

JsonObject::node_type *JsonObject::getNodeAt(JsonObjectKey key) const {
  for (node_type *node = _firstNode; node; node = node->next) {
    if (!strcmp(node->content.key, key)) return node;
  }
  return NULL;
}

size_t JsonObject::writeTo(JsonWriter &writer) const {
  size_t written = 0;
  if((written = writer.beginObject()) == 0)
    return 0;

  const node_type *node = _firstNode;
  size_t total = written;
  while (node) {
    if((written = writer.writeString(node->content.key)) == 0)
      break;
    total += written;
    if((written = writer.writeColon()) == 0)
      break;
    total += written;
    if((written = node->content.value.writeTo(writer)) == 0)
      break;
    total += written;

    node = node->next;
    if (!node) break;

    if((written = writer.writeComma()) == 0)
      break;
    total += written;
  }

  if(written)
    total += writer.endObject();
  return total;
}
