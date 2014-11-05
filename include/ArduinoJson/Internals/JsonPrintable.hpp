// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "IndentedPrint.hpp"
#include "PrettyJsonWriter.hpp"
#include "StringBuilder.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename T>
class JsonPrintable {
 public:
  size_t printTo(Print &print) const {
    JsonWriter writer(&print);
    downcast().writeTo(writer);
    return writer.bytesWritten();
  }
  size_t printTo(char *buffer, size_t bufferSize) const {
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
  }

  size_t prettyPrintTo(IndentedPrint &print) const {
    PrettyJsonWriter writer(&print);
    downcast().writeTo(writer);
    return writer.bytesWritten();
  }

  size_t prettyPrintTo(char *buffer, size_t bufferSize) const {
    StringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
  }
  size_t prettyPrintTo(Print &print) const {
    IndentedPrint indentedPrint = IndentedPrint(print);
    return prettyPrintTo(indentedPrint);
  }

 private:
  const T &downcast() const { return *static_cast<const T *>(this); }
};
}
}
