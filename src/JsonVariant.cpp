// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonVariant.hpp"

#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

void JsonVariant::writeTo(JsonWriter &writer) const {
  if (is<const JsonArray &>())
    as<const JsonArray &>().writeTo(writer);
  else if (is<const JsonObject &>())
    as<const JsonObject &>().writeTo(writer);
  else if (is<const char *>())
    writer.writeString(as<const char *>());
  else if (is<long>())
    writer.writeLong(as<long>());
  else if (is<bool>())
    writer.writeBoolean(as<bool>());
  else if (is<double>()) {
    uint8_t decimals = static_cast<uint8_t>(_type - JSON_DOUBLE_0_DECIMALS);
    writer.writeDouble(as<double>(), decimals);
  }
}
