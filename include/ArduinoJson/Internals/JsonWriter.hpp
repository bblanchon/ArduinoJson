// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Arduino/Print.hpp"
#include "../Polyfills/isNaN.hpp"
#include "../Polyfills/isInfinity.hpp"
#include "../Polyfills/normalize.hpp"
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

  void writeFloat(JsonFloat value, int digits = 2) {
    if (Polyfills::isNaN(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (Polyfills::isInfinity(value)) return writeRaw("Infinity");

    short powersOf10;
    if (value > 1000 || value < 0.001) {
      powersOf10 = Polyfills::normalize(value);
    } else {
      powersOf10 = 0;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    JsonFloat rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i) rounding /= 10.0;

    value += rounding;

    // Extract the integer part of the value and print it
    JsonUInt int_part = static_cast<JsonUInt>(value);
    JsonFloat remainder = value - static_cast<JsonFloat>(int_part);
    writeInteger(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
      writeRaw('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
      remainder *= 10.0;
      JsonUInt toPrint = JsonUInt(remainder);
      writeInteger(JsonUInt(remainder));
      remainder -= static_cast<JsonFloat>(toPrint);
    }

    if (powersOf10 < 0) {
      writeRaw("e-");
      writeInteger(-powersOf10);
    }

    if (powersOf10 > 0) {
      writeRaw('e');
      writeInteger(powersOf10);
    }
  }

  void writeInteger(JsonUInt value) {
    char buffer[22];

    uint8_t i = 0;
    do {
      buffer[i++] = static_cast<char>(value % 10 + '0');
      value /= 10;
    } while (value);

    while (i > 0) {
      writeRaw(buffer[--i]);
    }
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
