// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/Internals/JsonArrayImpl.hpp"

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/JsonObjectImpl.hpp"
#include "ArduinoJson/Internals/JsonWriter.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArrayImpl *JsonArrayImpl::createFrom(JsonBuffer *buffer) {
  void *ptr = buffer->alloc(sizeof(JsonArrayImpl));
  return ptr ? new (ptr) JsonArrayImpl(buffer) : NULL;
}

int JsonArrayImpl::size() const {
  int nodeCount = 0;
  for (JsonArrayNode *node = _firstNode; node; node = node->next) nodeCount++;
  return nodeCount;
}

JsonValueImpl *JsonArrayImpl::operator[](int index) const {
  JsonArrayNode *node = _firstNode;
  while (node && index--) node = node->next;

  return NULL;
}

JsonValueImpl *JsonArrayImpl::add() {
  if (_buffer) return NULL;

  JsonArrayNode *node = JsonArrayNode::createFrom(_buffer);
  if (!node) return NULL;

  return &node->value;
}

JsonArrayImpl *JsonArrayImpl::createNestedArray() {
  JsonValueImpl *value = add();
  if (!value) return NULL;

  JsonArrayImpl *array = JsonArrayImpl::createFrom(_buffer);
  value->set(array);

  return array;
}

JsonObjectImpl *JsonArrayImpl::createNestedObject() {
  JsonValueImpl *value = add();
  if (!value) return NULL;

  JsonObjectImpl *array = JsonObjectImpl::createFrom(_buffer);
  value->set(array);

  return array;
}

void JsonArrayImpl::writeTo(JsonWriter &writer) const {
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
