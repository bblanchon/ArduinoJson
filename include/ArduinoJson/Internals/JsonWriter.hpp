// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"
#include "QuotedString.hpp"

namespace ArduinoJson {
namespace Internals {

// Writes the JSON tokens to a Print implementation
// This class is used by:
// - JsonArray::writeTo()
// - JsonObject::writeTo()
// - JsonVariant::writeTo()
// Its derived by PrettyJsonWriter that overrides some members to add
// indentation.
class JsonWriter {
 public:
  explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

  // Returns the number of bytes sent to the Print implementation.
  // This is very handy for implementations of printTo() that must return the
  // number of bytes written.
  size_t bytesWritten() { return _length; }

  void beginArray() { write('['); }
  void endArray() { write(']'); }

  void beginObject() { write('{'); }
  void endObject() { write('}'); }

  void writeColon() { write(':'); }
  void writeComma() { write(','); }

  void writeString(const char *value) {
    _length += QuotedString::printTo(value, _sink);
  }

  void writeLong(long value) { _length += _sink.print(value); }

  void writeBoolean(bool value) {
    _length += _sink.print(value ? "true" : "false");
  }
  void writeDouble(double value, uint8_t decimals) {
    _length += _sink.print(value, decimals);
  }

 protected:
  void write(char c) { _length += _sink.write(c); }
  void write(const char *s) { _length += _sink.print(s); }

  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}
}
