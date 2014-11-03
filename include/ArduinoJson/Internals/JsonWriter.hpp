// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonWriter {
 public:
  explicit JsonWriter(Print *sink) : _sink(sink), _length(0) {}

  size_t bytesWritten() { return _length; }

  void writeEmptyArray() { _length += _sink->print("[]"); }

  void writeEmptyObject() { _length += _sink->print("{}"); }

  void writeString(const char *value);
  void writeInteger(long value);
  void writeBoolean(bool value);
  void writeDouble(double value, int decimals);

 protected:
  Print *_sink;
  size_t _length;
};
}
}
