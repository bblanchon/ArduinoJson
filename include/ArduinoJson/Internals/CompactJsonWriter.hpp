// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonWriter.hpp"

namespace ArduinoJson {
namespace Internals {

class CompactJsonWriter : public JsonWriter {
 public:
  explicit CompactJsonWriter(Print *sink) : JsonWriter(sink) {}

  void beginArray() { _length += _sink->write('['); }
  void endArray() { _length += _sink->write(']'); }

  void beginObject() { _length += _sink->write('{'); }
  void endObject() { _length += _sink->write('}'); }

  void writeColon() { _length += _sink->write(':'); }
  void writeComma() { _length += _sink->write(','); }
};
}
}
