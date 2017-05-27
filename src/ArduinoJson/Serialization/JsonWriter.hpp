// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <stdint.h>
#include "../Data/Encoding.hpp"
#include "../Data/JsonFloat.hpp"
#include "../Data/JsonInteger.hpp"
#include "../Polyfills/attributes.hpp"
#include "../Polyfills/math.hpp"
#include "../Polyfills/normalize.hpp"
#include "../TypeTraits/FloatTraits.hpp"

namespace ArduinoJson {
namespace Internals {

// Writes the JSON tokens to a Print implementation
// This class is used by:
// - JsonArray::writeTo()
// - JsonObject::writeTo()
// - JsonVariant::writeTo()
// Its derived by PrettyJsonWriter that overrides some members to add
// indentation.
template <typename Print>
class JsonWriter {
  static const uint8_t maxDecimalPlaces = sizeof(JsonFloat) >= 8 ? 9 : 6;
  static const uint32_t maxDecimalPart =
      sizeof(JsonFloat) >= 8 ? 1000000000 : 1000000;

 public:
  explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

  // Returns the number of bytes sent to the Print implementation.
  // This is very handy for implementations of printTo() that must return the
  // number of bytes written.
  size_t bytesWritten() const {
    return _length;
  }

  void beginArray() {
    writeRaw('[');
  }
  void endArray() {
    writeRaw(']');
  }

  void beginObject() {
    writeRaw('{');
  }
  void endObject() {
    writeRaw('}');
  }

  void writeColon() {
    writeRaw(':');
  }
  void writeComma() {
    writeRaw(',');
  }

  void writeBoolean(bool value) {
    writeRaw(value ? "true" : "false");
  }

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

  void writeFloat(JsonFloat value) {
    if (Polyfills::isNaN(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (Polyfills::isInfinity(value)) return writeRaw("Infinity");

    uint32_t integralPart, decimalPart;
    int16_t powersOf10;
    splitFloat(value, integralPart, decimalPart, powersOf10);

    writeInteger(integralPart);
    if (decimalPart) writeDecimals(decimalPart, maxDecimalPlaces);

    if (powersOf10 < 0) {
      writeRaw("e-");
      writeInteger(-powersOf10);
    }

    if (powersOf10 > 0) {
      writeRaw('e');
      writeInteger(powersOf10);
    }
  }

  template <typename UInt>
  void writeInteger(UInt value) {
    char buffer[22];
    char *ptr = buffer + sizeof(buffer) - 1;

    *ptr = 0;
    do {
      *--ptr = char(value % 10 + '0');
      value = UInt(value / 10);
    } while (value);

    writeRaw(ptr);
  }

  void writeDecimals(uint32_t value, int8_t width) {
    // remove trailing zeros
    while (value % 10 == 0 && width > 0) {
      value /= 10;
      width--;
    }

    // buffer should be big enough for all digits, the dot and the null
    // terminator
    char buffer[maxDecimalPlaces + 2];
    char *ptr = buffer + sizeof(buffer) - 1;

    // write the string in reverse order
    *ptr = 0;
    while (width--) {
      *--ptr = char(value % 10 + '0');
      value /= 10;
    }
    *--ptr = '.';

    // and dump it in the right order
    writeRaw(ptr);
  }

  void writeRaw(const char *s) {
    _length += _sink.print(s);
  }
  void writeRaw(char c) {
    _length += _sink.print(c);
  }

 protected:
  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned

  void splitFloat(JsonFloat value, uint32_t &integralPart,
                  uint32_t &decimalPart, int16_t &powersOf10) {
    powersOf10 = Polyfills::normalize(value);

    integralPart = uint32_t(value);
    JsonFloat remainder = value - JsonFloat(integralPart);

    remainder *= maxDecimalPart;
    decimalPart = uint32_t(remainder);
    remainder = remainder - JsonFloat(decimalPart);

    // rounding:
    // increment by 1 if remainder >= 0.5
    decimalPart += uint32_t(remainder * 2);
    if (decimalPart >= maxDecimalPart) {
      decimalPart = 0;
      integralPart++;
      if (powersOf10 && integralPart >= 10) {
        powersOf10++;
        integralPart = 1;
      }
    }
  }
};
}
}
