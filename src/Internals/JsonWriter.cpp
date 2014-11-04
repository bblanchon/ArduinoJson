// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/JsonWriter.hpp"

#include "../../include/ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson::Internals;

void JsonWriter::writeString(char const *value) {
  _length += QuotedString::printTo(value, _sink);
}

void JsonWriter::writeInteger(long value) { _length += _sink->print(value); }

void JsonWriter::writeBoolean(bool value) {
  _length += _sink->print(value ? "true" : "false");
}

void JsonWriter::writeDouble(double value, uint8_t decimals) {
  _length += _sink->print(value, decimals);
}
