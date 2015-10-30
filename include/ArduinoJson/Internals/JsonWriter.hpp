// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

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

  void beginArray() { write('['); }
  void endArray() { write(']'); }

  void beginObject() { write('{'); }
  void endObject() { write('}'); }

  void writeColon() { write(':'); }
  void writeComma() { write(','); }

  void writeBoolean(bool value) { write(value ? "true" : "false"); }

  void writeString(const char *value) {
    if (!value) {
      write("null");
    } else {
      write('\"');
      while (*value) writeChar(*value++);
      write('\"');
    }
  }

  void writeChar(char c) {
    char specialChar = Encoding::escapeChar(c);
    if (specialChar) {
      write('\\');
      write(specialChar);
    } else {
      write(c);
    }
  }

  void writeInteger(JsonInteger value) { _length += _sink.print(value); }

  void writeFloat(JsonFloat value, uint8_t decimals) {
    _length += _sink.print(value, decimals);
  }

  void writeRaw(const char *s) { return write(s); }

 protected:
  void write(char c) { _length += _sink.write(c); }
  FORCE_INLINE void write(const char *s) { _length += _sink.print(s); }

  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}
}
