// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "ArduinoJson/JsonPrintable.hpp"

#include "ArduinoJson/JsonBuffer.hpp"
#include "ArduinoJson/Internals/StringBuilder.hpp"
#include "ArduinoJson/Internals/CompactJsonWriter.hpp"
#include "ArduinoJson/Internals/PrettyJsonWriter.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

size_t JsonPrintable::printTo(char *buffer, size_t bufferSize) const {
  StringBuilder sb(buffer, bufferSize);
  return printTo(sb);
}

size_t JsonPrintable::printTo(Print &p) const {
  CompactJsonWriter writer(&p);
  writeTo(writer);
  return writer.bytesWritten();
}

size_t JsonPrintable::prettyPrintTo(char *buffer, size_t bufferSize) const {
  StringBuilder sb(buffer, bufferSize);
  return prettyPrintTo(sb);
}

size_t JsonPrintable::prettyPrintTo(IndentedPrint &p) const {
  PrettyJsonWriter writer(&p);
  writeTo(writer);
  return writer.bytesWritten();
}

size_t JsonPrintable::prettyPrintTo(Print &print) const {
  IndentedPrint indentedPrint = IndentedPrint(print);
  return prettyPrintTo(indentedPrint);
}
