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

  size_t beginArray() { return write('['); }
  size_t endArray() { return write(']'); }

  size_t beginObject() { return write('{'); }
  size_t endObject() { return write('}'); }

  size_t writeColon() { return write(':'); }
  size_t writeComma() { return write(','); }

  size_t writeBoolean(bool value) { return write(value ? "true" : "false"); }

  size_t writeString(const char *value) {
    size_t written = 0;
    if (!value) {
      written = write("null");
    } else {
      written = write('\"');
      while (*value && writeChar(*value++) == 1) written++;
      written += write('\"');
    }
    return written;
  }

  size_t writeChar(char c) {
    char specialChar = Encoding::escapeChar(c);
    size_t written = 0;
    if (specialChar) {
      if(write('\\') == 0)
        return 0;
      written = write(specialChar);
    } else {
      written = write(c);
    }
    return written;
  }

  size_t writeInteger(JsonInteger value) {
    size_t written = _sink.print(value);
    _length += written;
    return written;
  }

  size_t writeFloat(JsonFloat value, uint8_t decimals) {
    size_t written = _sink.print(value, decimals);
    _length += written;
    return written;
  }

  size_t writeRaw(const char *s) { return write(s); }

 protected:
  size_t write(char c) { size_t written = _sink.write(c); _length += written; return written; }
  FORCE_INLINE size_t write(const char *s) { size_t written = _sink.print(s); _length += written; return written; }

  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}
}
