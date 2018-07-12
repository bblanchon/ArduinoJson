// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdint.h>
#include <string.h>  // for strlen
#include "../Data/JsonInteger.hpp"
#include "../Numbers/FloatParts.hpp"
#include "../Polyfills/attributes.hpp"
#include "./EscapeSequence.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TWriter>
class JsonWriter {
 public:
  explicit JsonWriter(TWriter &writer) : _writer(writer), _length(0) {}

  // Returns the number of bytes sent to the TWriter implementation.
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
    if (value)
      writeRaw("true");
    else
      writeRaw("false");
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
    char specialChar = EscapeSequence::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }

  template <typename TFloat>
  void writeFloat(TFloat value) {
    if (isnan(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (isinf(value)) return writeRaw("Infinity");

    FloatParts<TFloat> parts(value);

    writeInteger(parts.integral);
    if (parts.decimalPlaces) writeDecimals(parts.decimal, parts.decimalPlaces);

    if (parts.exponent < 0) {
      writeRaw("e-");
      writeInteger(-parts.exponent);
    }

    if (parts.exponent > 0) {
      writeRaw('e');
      writeInteger(parts.exponent);
    }
  }

  template <typename UInt>
  void writeInteger(UInt value) {
    char buffer[22];
    char *end = buffer + sizeof(buffer);
    char *begin = end;

    // write the string in reverse order
    do {
      *--begin = char(value % 10 + '0');
      value = UInt(value / 10);
    } while (value);

    // and dump it in the right order
    writeRaw(begin, end);
  }

  void writeDecimals(uint32_t value, int8_t width) {
    // buffer should be big enough for all digits and the dot
    char buffer[16];
    char *end = buffer + sizeof(buffer);
    char *begin = end;

    // write the string in reverse order
    while (width--) {
      *--begin = char(value % 10 + '0');
      value /= 10;
    }
    *--begin = '.';

    // and dump it in the right order
    writeRaw(begin, end);
  }

  void writeRaw(const char *s) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(s), strlen(s));
  }
  void writeRaw(const char *begin, const char *end) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(begin),
                             static_cast<size_t>(end - begin));
  }
  template <size_t N>
  void writeRaw(const char (&s)[N]) {
    _length += _writer.write(reinterpret_cast<const uint8_t *>(s), N - 1);
  }
  void writeRaw(char c) {
    _length += _writer.write(static_cast<uint8_t>(c));
  }

 protected:
  TWriter &_writer;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}  // namespace Internals
}  // namespace ArduinoJson
