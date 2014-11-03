// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Arduino/Printable.hpp"
#include "Internals/StringBuilder.hpp"
#include "Internals/IndentedPrint.hpp"

namespace ArduinoJson {

namespace Internals {
class IndentedPrint;
}

template <typename T>
class JsonPrintable : public Printable {
 public:
  size_t printTo(Print &print) const {
    CompactJsonWriter writer(&p);
    downcast().writeTo(writer);
    return writer.bytesWritten();
  }
  size_t printTo(char *buffer, size_t bufferSize) const {
    Internals::StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
  }

  size_t prettyPrintTo(Internals::IndentedPrint &print) const {
    PrettyJsonWriter writer(&p);
    downcast().writeTo(writer);
    return writer.bytesWritten();
  }

  size_t prettyPrintTo(char *buffer, size_t bufferSize) const {
    Internals::StringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
  }
  size_t prettyPrintTo(Print &print) const {
    Internals::IndentedPrint indentedPrint = Internals::IndentedPrint(print);
    return prettyPrintTo(indentedPrint);
  }

 private:
  const T &downcast() { return *static_cast<const T *>(this); }
};
}
