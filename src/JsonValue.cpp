// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/Internals/PrettyJsonWriter.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValue JsonValue::_invalid(JSON_INVALID);

JsonValue::operator JsonArray &() const {
  return _type == JSON_ARRAY ? *_content.asArray : JsonArray::invalid();
}

JsonValue::operator JsonObject &() const {
  return _type == JSON_OBJECT ? *_content.asObject : JsonObject::invalid();
}

JsonValue::operator bool() const {
  return _type == JSON_BOOLEAN ? _content.asBoolean : false;
}

JsonValue::operator const char *() const {
  return _type == JSON_STRING ? _content.asString : NULL;
}

JsonValue::operator double() const {
  return _type >= JSON_DOUBLE_0_DECIMALS ? _content.asDouble : 0;
}

JsonValue::operator long() const {
  return _type == JSON_LONG ? _content.asLong : 0;
}

void JsonValue::set(bool value) {
  if (_type == JSON_INVALID) return;
  _type = Internals::JSON_BOOLEAN;
  _content.asBoolean = value;
}

void JsonValue::set(const char *value) {
  if (_type == JSON_INVALID) return;
  _type = JSON_STRING;
  _content.asString = value;
}

void JsonValue::set(double value, int decimals) {
  if (_type == JSON_INVALID) return;
  _type = static_cast<JsonValueType>(JSON_DOUBLE_0_DECIMALS + decimals);
  _content.asDouble = value;
}

void JsonValue::set(long value) {
  if (_type == JSON_INVALID) return;
  _type = JSON_LONG;
  _content.asLong = value;
}

void JsonValue::set(JsonArray &array) {
  if (_type == JSON_INVALID) return;
  _type = JSON_ARRAY;
  _content.asArray = &array;
}

void JsonValue::set(JsonObject &object) {
  if (_type == JSON_INVALID) return;
  _type = JSON_OBJECT;
  _content.asObject = &object;
}

template <typename T>
void JsonValue::writeTo(T &writer) const {
  switch (_type) {
    case JSON_ARRAY:
      _content.asArray->writeTo(writer);
      break;

    case JSON_OBJECT:
      _content.asObject->writeTo(writer);
      break;

    case JSON_STRING:
      writer.writeString(_content.asString);
      break;

    case JSON_LONG:
      writer.writeInteger(_content.asLong);
      break;

    case JSON_BOOLEAN:
      writer.writeBoolean(_content.asBoolean);
      break;

    default:  // >= JSON_DOUBLE_0_DECIMALS
      writer.writeDouble(_content.asDouble, _type - JSON_DOUBLE_0_DECIMALS);
      break;
  }
}

template void JsonValue::writeTo(JsonWriter &) const;
template void JsonValue::writeTo(PrettyJsonWriter &) const;
