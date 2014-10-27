// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonBuffer.hpp"

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/Internals/JsonArrayImpl.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/Internals/JsonObjectImpl.hpp"
#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/Internals/JsonValueImpl.hpp"
#include "ArduinoJson/Internals/JsonParser.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

// TODO: what happens if alloc returns NULL
void* operator new(size_t size, ArduinoJson::JsonBuffer* buffer) {
  return buffer->alloc(size);
}

JsonArray JsonBuffer::createArray() {
  return JsonArray(new (this) JsonArrayImpl(this));
}

JsonObject JsonBuffer::createObject() {
  return JsonObject(new (this) JsonObjectImpl(this));
}

JsonValue JsonBuffer::createValue() {
  return JsonValue(new (this) JsonValueImpl());
}

JsonArray JsonBuffer::parseArray(char* json) {
  JsonParser parser(this, json);
  return JsonArray(parser.parseArray());
}

JsonObject JsonBuffer::parseObject(char* json) {
  JsonParser parser(this, json);
  return JsonObject(parser.parseObject());
}

JsonValue JsonBuffer::parseValue(char* json) {
  JsonParser parser(this, json);
  return JsonValue(parser.parseValue());
}