// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Arduino/Print.hpp"
#include "Encoding.hpp"
#include "ForceInline.hpp"
#include "JsonFloat.hpp"
#include "JsonInteger.hpp"

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
  size_t bytesWritten() const { return _length; }

  void beginArray() { writeRaw('['); }
  void endArray() { writeRaw(']'); }

  void beginObject() { writeRaw('{'); }
  void endObject() { writeRaw('}'); }

  void writeColon() { writeRaw(':'); }
  void writeComma() { writeRaw(','); }

  void writeBoolean(bool value) { writeRaw(value ? "true" : "false"); }

  void writeString(const char *value) {
    if (!value) {
      writeRaw("null");
    } else {
      writeRaw('\"');
      while (*value) writeChar(*value++);
      writeRaw('\"');
    }
  }

  void writeChar(char c) {
    char specialChar = Encoding::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }

  void writeInteger(JsonUInt value) { _length += _sink.print(value); }

  void writeFloat(JsonFloat value, uint8_t decimals) {
    _length += _sink.print(value, decimals);
  }

  void writeRaw(const char *s) { _length += _sink.print(s); }
  void writeRaw(char c) { _length += _sink.write(c); }

 protected:
  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}
}
