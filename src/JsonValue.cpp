#include "ArduinoJson/JsonValue.hpp"

#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/Internals/JsonNode.hpp"

using namespace ArduinoJson;

void JsonValue::operator=(bool value) {
  if (_node)
    _node->setAsBoolean(value);
}

void JsonValue::operator=(char const *value) {
  if (_node)
    _node->setAsString(value);
}

void JsonValue::set(double value, int decimals) {
  if (_node)
    _node->setAsDouble(value, decimals);
}

void JsonValue::operator=(int value) {
  if (_node)
    _node->setAsLong(value);
}

JsonValue::operator bool() const {
  return _node ? _node->getAsBoolean() : false;
}

JsonValue::operator char const *() const {
  return _node ? _node->getAsString() : 0;
}

JsonValue::operator double() const { return _node ? _node->getAsDouble() : 0; }

JsonValue::operator long() const { return _node ? _node->getAsInteger() : 0; }

JsonValue::operator JsonArray() const { return JsonArray(_node); }

JsonValue::operator JsonObject() const { return JsonObject(_node); }