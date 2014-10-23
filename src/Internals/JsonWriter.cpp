#include "ArduinoJson/Internals/JsonWriter.hpp"
#include "ArduinoJson/Internals/QuotedString.hpp"

using namespace ArduinoJson::Internals;

void JsonWriter::writeString(char const *value) {
  _length += QuotedString::printTo(value, _sink);
}

void JsonWriter::writeInteger(long value) { _length += _sink->print(value); }

void JsonWriter::writeBoolean(bool value) {
  _length += _sink->print(value ? "true" : "false");
}

void JsonWriter::writeDouble(double value, int decimals) {
  _length += _sink->print(value, decimals);
}