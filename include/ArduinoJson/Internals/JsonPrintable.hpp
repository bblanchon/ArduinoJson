// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "DummyPrint.hpp"
#include "IndentedPrint.hpp"
#include "JsonWriter.hpp"
#include "Prettyfier.hpp"
#include "StaticStringBuilder.hpp"
#include "DynamicStringBuilder.hpp"

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
#include "StreamPrintAdapter.hpp"
#endif

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

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
  std::ostream &printTo(std::ostream &os) const {
    StreamPrintAdapter adapter(os);
    printTo(adapter);
    return os;
  }
#endif

  size_t printTo(char *buffer, size_t bufferSize) const {
    StaticStringBuilder sb(buffer, bufferSize);
    return printTo(sb);
  }

  size_t printTo(String &str) const {
    DynamicStringBuilder sb(str);
    return printTo(sb);
  }

  size_t prettyPrintTo(IndentedPrint &print) const {
    Prettyfier p(print);
    return printTo(p);
  }

  size_t prettyPrintTo(char *buffer, size_t bufferSize) const {
    StaticStringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
  }

  size_t prettyPrintTo(Print &print) const {
    IndentedPrint indentedPrint = IndentedPrint(print);
    return prettyPrintTo(indentedPrint);
  }

  size_t prettyPrintTo(String &str) const {
    DynamicStringBuilder sb(str);
    return prettyPrintTo(sb);
  }

  size_t measureLength() const {
    DummyPrint dp;
    return printTo(dp);
  }

  size_t measurePrettyLength() const {
    DummyPrint dp;
    return prettyPrintTo(dp);
  }

 private:
  const T &downcast() const { return *static_cast<const T *>(this); }
};

#ifdef ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const JsonPrintable<T> &v) {
  return v.printTo(os);
}
#endif
}
}
