// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "IndentedPrint.hpp"
#include "JsonWriter.hpp"
#include "Prettyfier.hpp"
#include "StringBuilder.hpp"

namespace ArduinoJson {
namespace Internals {

// Implements all the overloads of printTo() and prettyPrintTo()
// Caution: this class use a template parameter to avoid virtual methods.
// This is a bit curious but allows to reduce the size of JsonVariant, JsonArray
// and JsonObject.
template <typename T>
class JsonPrintable {
 public:
  size_t printTo(Print &print) const {
    JsonWriter writer(print);
    downcast().writeTo(writer);
    return writer.bytesWritten();
  }

  size_t printTo(char *buffer, size_t bufferSize) const {
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
  }

  size_t prettyPrintTo(IndentedPrint &print) const {
    Prettyfier p(print);
    return printTo(p);
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
