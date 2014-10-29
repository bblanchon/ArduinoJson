// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonBuffer.hpp"

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/Internals/JsonParser.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonArray &JsonBuffer::createArray() {
  void *ptr = alloc(sizeof(JsonArray));
  if (ptr) return *new (ptr) JsonArray(this);
  return JsonArray::invalid();
}

JsonObject &JsonBuffer::createObject() {
  void *ptr = alloc(sizeof(JsonObject));
  if (ptr) return *new (ptr) JsonObject(this);
  return JsonObject::invalid();
}

JsonArray &JsonBuffer::parseArray(char *json) {
  JsonParser parser(this, json);
  return parser.parseArray();
}

JsonObject &JsonBuffer::parseObject(char *json) {
  JsonParser parser(this, json);
  return parser.parseObject();
}
