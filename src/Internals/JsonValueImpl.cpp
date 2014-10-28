// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/Internals/JsonValueImpl.hpp"
#include "ArduinoJson/Internals/JsonArrayImpl.hpp"
#include "ArduinoJson/Internals/JsonObjectImpl.hpp"
#include "ArduinoJson/Internals/JsonWriter.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValueImpl* JsonValueImpl::createFrom(JsonBuffer* buffer) {
  void* ptr = buffer->alloc(sizeof(JsonValueImpl));
  return ptr ? new (ptr) JsonValueImpl() : NULL;
}

void JsonValueImpl::writeTo(JsonWriter& writer) const {
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
      writer.writeInteger(_content.asInteger);
      break;

    case JSON_BOOLEAN:
      writer.writeBoolean(_content.asBoolean);
      break;

    default:  // >= JSON_DOUBLE_0_DECIMALS
      writer.writeDouble(_content.asDouble, _type - JSON_DOUBLE_0_DECIMALS);
      break;
  }
}