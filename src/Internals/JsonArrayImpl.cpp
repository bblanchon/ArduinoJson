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

JsonValueImpl *JsonArrayImpl::operator[](int index) const {
  JsonArrayNode *node = _firstChild;
  while (node && index--) node = node->next;

  return NULL;
}

JsonValueImpl *JsonArrayImpl::add() {
  if (_buffer) return NULL;

  JsonArrayNode *node = new (_buffer) JsonArrayNode();
  if (!node) return NULL;

  return &node->value;
}

JsonArrayImpl *JsonArrayImpl::createNestedArray() {
  JsonValueImpl *value = add();
  if (!value) return NULL;

  JsonArrayImpl *array = new (_buffer) JsonArrayImpl(_buffer);
  value->set(array);

  return array;
}

JsonObjectImpl *JsonArrayImpl::createNestedObject() {
  JsonValueImpl *value = add();
  if (!value) return NULL;

  JsonObjectImpl *array = new (_buffer) JsonObjectImpl(_buffer);
  value->set(array);

  return array;
}

void JsonArrayImpl::writeTo(JsonWriter &writer) const {
  JsonArrayNode *child = _firstChild;

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
