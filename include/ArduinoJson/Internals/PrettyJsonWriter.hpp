// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonWriter.hpp"
#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {

// An indented version of JsonWriter.
class PrettyJsonWriter : public JsonWriter {
 public:
  explicit PrettyJsonWriter(IndentedPrint &sink)
      : JsonWriter(sink), _indenter(sink) {}

  void beginArray() {
    JsonWriter::beginArray();
    indent();
  }

  void endArray() {
    unindent();
    JsonWriter::endArray();
  }

  void writeColon() { write(": "); }

  void writeComma() {
    JsonWriter::writeComma();
    newline();
  }

  void beginObject() {
    JsonWriter::beginObject();
    indent();
  }

  void endObject() {
    unindent();
    JsonWriter::endObject();
  }

 private:
  void indent() {
    _indenter.indent();
    newline();
  }

  void unindent() {
    newline();
    _indenter.unindent();
  }

  void newline() { _length += _indenter.println(); }

  IndentedPrint &_indenter;
};
}
}
