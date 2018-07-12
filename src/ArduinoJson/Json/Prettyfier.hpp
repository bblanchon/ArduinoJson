// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {

// Converts a compact JSON string into an indented one.
template <typename TWriter>
class Prettyfier {
 public:
  explicit Prettyfier(IndentedPrint<TWriter>& p) : _sink(p) {
    _previousChar = 0;
    _inString = false;
  }

  size_t write(uint8_t c) {
    size_t n = _inString ? handleStringChar(c) : handleMarkupChar(char(c));
    _previousChar = char(c);
    return n;
  }

  size_t write(const uint8_t* s, size_t n) {
    // TODO: optimize
    size_t bytesWritten = 0;
    while (n > 0) {
      bytesWritten += write(*s++);
      n--;
    }
    return bytesWritten;
  }

 private:
  Prettyfier& operator=(const Prettyfier&);  // cannot be assigned

  bool inEmptyBlock() {
    return _previousChar == '{' || _previousChar == '[';
  }

  size_t handleStringChar(uint8_t c) {
    bool isQuote = c == '"' && _previousChar != '\\';

    if (isQuote) _inString = false;

    return _sink.write(c);
  }

  size_t handleMarkupChar(char c) {
    switch (c) {
      case '{':
      case '[':
        return writeBlockOpen(c);

      case '}':
      case ']':
        return writeBlockClose(c);

      case ':':
        return writeColon();

      case ',':
        return writeComma();

      case '"':
        return writeQuoteOpen();

      default:
        return writeNormalChar(c);
    }
  }

  size_t writeBlockClose(char c) {
    size_t n = 0;
    n += unindentIfNeeded();
    n += write(c);
    return n;
  }

  size_t writeBlockOpen(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += write(c);
    return n;
  }

  size_t writeColon() {
    size_t n = 0;
    n += write(": ");
    return n;
  }

  size_t writeComma() {
    size_t n = 0;
    n += write(",\r\n");
    return n;
  }

  size_t writeQuoteOpen() {
    _inString = true;
    size_t n = 0;
    n += indentIfNeeded();
    n += write('"');
    return n;
  }

  size_t writeNormalChar(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += write(c);
    return n;
  }

  size_t indentIfNeeded() {
    if (!inEmptyBlock()) return 0;

    _sink.indent();
    return write("\r\n");
  }

  size_t unindentIfNeeded() {
    if (inEmptyBlock()) return 0;

    _sink.unindent();
    return write("\r\n");
  }

  size_t write(char c) {
    return _sink.write(static_cast<uint8_t>(c));
  }

  template <size_t N>
  size_t write(const char (&s)[N]) {
    return _sink.write(reinterpret_cast<const uint8_t*>(s), N - 1);
  }

  char _previousChar;
  IndentedPrint<TWriter>& _sink;
  bool _inString;
};
}  // namespace Internals
}  // namespace ArduinoJson
