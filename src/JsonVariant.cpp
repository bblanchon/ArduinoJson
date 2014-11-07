// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonVariant.hpp"

#include "../include/ArduinoJson/Internals/PrettyJsonWriter.hpp"
#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonVariant JsonVariant::_invalid(JSON_INVALID);

JsonVariant::operator JsonArray &() const {
  return _type == JSON_ARRAY ? *_content.asArray : JsonArray::invalid();
}

JsonVariant::operator JsonObject &() const {
  return _type == JSON_OBJECT ? *_content.asObject : JsonObject::invalid();
}

JsonVariant::operator bool() const {
  return _type == JSON_BOOLEAN ? _content.asBoolean : false;
}

JsonVariant::operator const char *() const {
  return _type == JSON_STRING ? _content.asString : NULL;
}

JsonVariant::operator double() const {
  return _type >= JSON_DOUBLE_0_DECIMALS ? _content.asDouble : 0;
}

JsonVariant::operator long() const {
  return _type == JSON_LONG ? _content.asLong : 0;
}

void JsonVariant::set(bool value) {
  if (_type == JSON_INVALID) return;
  _type = Internals::JSON_BOOLEAN;
  _content.asBoolean = value;
}

void JsonVariant::set(const char *value) {
  if (_type == JSON_INVALID) return;
  _type = JSON_STRING;
  _content.asString = value;
}

void JsonVariant::set(double value, uint8_t decimals) {
  if (_type == JSON_INVALID) return;
  _type = static_cast<JsonVariantType>(JSON_DOUBLE_0_DECIMALS + decimals);
  _content.asDouble = value;
}

void JsonVariant::set(long value) {
  if (_type == JSON_INVALID) return;
  _type = JSON_LONG;
  _content.asLong = value;
}

void JsonVariant::set(JsonArray &array) {
  if (_type == JSON_INVALID) return;
  _type = JSON_ARRAY;
  _content.asArray = &array;
}

void JsonVariant::set(JsonObject &object) {
  if (_type == JSON_INVALID) return;
  _type = JSON_OBJECT;
  _content.asObject = &object;
}

size_t JsonVariant::size() const {
  if (_type == JSON_ARRAY) return _content.asArray->size();
  if (_type == JSON_OBJECT) return _content.asObject->size();
  return 0;
}

JsonVariant &JsonVariant::operator[](int index) {
  if (_type != JSON_ARRAY) return JsonVariant::invalid();
  return _content.asArray->operator[](index);
}

JsonVariant &JsonVariant::operator[](const char *key) {
  if (_type != JSON_OBJECT) return JsonVariant::invalid();
  return _content.asObject->operator[](key);
}

template <typename T>
void JsonVariant::writeTo(T &writer) const {
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
      uint8_t decimals = static_cast<uint8_t>(_type - JSON_DOUBLE_0_DECIMALS);
      writer.writeDouble(_content.asDouble, decimals);
      break;
  }
}

template void JsonVariant::writeTo(JsonWriter &) const;
template void JsonVariant::writeTo(PrettyJsonWriter &) const;
