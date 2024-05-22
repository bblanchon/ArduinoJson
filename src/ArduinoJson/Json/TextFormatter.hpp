// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>
#include <string.h>  // for strlen

#include <ArduinoJson/Json/EscapeSequence.hpp>
#include <ArduinoJson/Numbers/FloatParts.hpp>
#include <ArduinoJson/Numbers/JsonInteger.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Serialization/CountingDecorator.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class TextFormatter {
 public:
  explicit TextFormatter(TWriter writer) : writer_(writer) {}

  TextFormatter& operator=(const TextFormatter&) = delete;

  // Returns the number of bytes sent to the TWriter implementation.
  size_t bytesWritten() const {
    return writer_.count();
  }

  void writeBoolean(bool value) {
    if (value)
      writeRaw("true");
    else
      writeRaw("false");
  }

  void writeString(const char* value) {
    ARDUINOJSON_ASSERT(value != NULL);
    writeRaw('\"');
    while (*value)
      writeChar(*value++);
    writeRaw('\"');
  }

  void writeString(const char* value, size_t n) {
    ARDUINOJSON_ASSERT(value != NULL);
    writeRaw('\"');
    while (n--)
      writeChar(*value++);
    writeRaw('\"');
  }

  void writeChar(char c) {
    char specialChar = EscapeSequence::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else if (c) {
      writeRaw(c);
    } else {
      writeRaw("\\u0000");
    }
  }

  template <typename T>
  void writeFloat(T value) {
    if (isnan(value))
      return writeRaw(ARDUINOJSON_ENABLE_NAN ? "NaN" : "null");

#if ARDUINOJSON_ENABLE_INFINITY
    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (isinf(value))
      return writeRaw("Infinity");
#else
    if (isinf(value))
      return writeRaw("null");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }
#endif

    FloatParts<T> parts(value);

    writeInteger(parts.integral);
    if (parts.decimalPlaces)
      writeDecimals(parts.decimal, parts.decimalPlaces);

    if (parts.exponent) {
      writeRaw('e');
      writeInteger(parts.exponent);
    }
  }

  template <typename T>
  enable_if_t<is_signed<T>::value> writeInteger(T value) {
    typedef make_unsigned_t<T> unsigned_type;
    unsigned_type unsigned_value;
    if (value < 0) {
      writeRaw('-');
      unsigned_value = unsigned_type(unsigned_type(~value) + 1);
    } else {
      unsigned_value = unsigned_type(value);
    }
    writeInteger(unsigned_value);
  }

  template <typename T>
  enable_if_t<is_unsigned<T>::value> writeInteger(T value) {
    char buffer[22];
    char* end = buffer + sizeof(buffer);
    char* begin = end;

    // write the string in reverse order
    do {
      *--begin = char(value % 10 + '0');
      value = T(value / 10);
    } while (value);

    // and dump it in the right order
    writeRaw(begin, end);
  }

  void writeDecimals(uint32_t value, int8_t width) {
    // buffer should be big enough for all digits and the dot
    char buffer[16];
    char* end = buffer + sizeof(buffer);
    char* begin = end;

    // write the string in reverse order
    while (width--) {
      *--begin = char(value % 10 + '0');
      value /= 10;
    }
    *--begin = '.';

    // and dump it in the right order
    writeRaw(begin, end);
  }

  void writeRaw(const char* s) {
    writer_.write(reinterpret_cast<const uint8_t*>(s), strlen(s));
  }

  void writeRaw(const char* s, size_t n) {
    writer_.write(reinterpret_cast<const uint8_t*>(s), n);
  }

  void writeRaw(const char* begin, const char* end) {
    writer_.write(reinterpret_cast<const uint8_t*>(begin),
                  static_cast<size_t>(end - begin));
  }

  template <size_t N>
  void writeRaw(const char (&s)[N]) {
    writer_.write(reinterpret_cast<const uint8_t*>(s), N - 1);
  }
  void writeRaw(char c) {
    writer_.write(static_cast<uint8_t>(c));
  }

 protected:
  CountingDecorator<TWriter> writer_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
