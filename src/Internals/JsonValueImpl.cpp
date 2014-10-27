// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/Internals/JsonValueImpl.hpp"

void JsonValueImpl::writeTo(JsonWriter &writer) const {
  switch (_type) {
    case JSON_ARRAY:
      _content.asArray->writeTo(writer);
      break;

    case JSON_OBJECT:
      _content.asObject->writeTo(writer);
      break;

    case JSON_STRING:
      writer.writeString(content.asString);
      break;

    case JSON_LONG:
      writer.writeInteger(content.asInteger);
      break;

    case JSON_BOOLEAN:
      writer.writeBoolean(content.asBoolean);
      break;

    default:  // >= JSON_DOUBLE_0_DECIMALS
      writer.writeDouble(content.asDouble, type - JSON_DOUBLE_0_DECIMALS);
      break;
  }
}