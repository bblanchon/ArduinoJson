#pragma once

#include "JsonWriter.hpp"
#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {
class PrettyJsonWriter : public JsonWriter {
 public:
  explicit PrettyJsonWriter(IndentedPrint *sink)
      : JsonWriter(sink), _indenter(sink) {}

  virtual void beginArray() {
    _length += _sink->write('[');
    indent();
  }

  virtual void endArray() {
    unindent();
    _length += _sink->write(']');
  }

  virtual void writeColon() { _length += _sink->print(": "); }

  virtual void writeComma() {
    _length += _sink->write(',');
    _length += _indenter->println();
  }

  virtual void beginObject() {
    _length += _sink->write('{');
    indent();
  }

  virtual void endObject() {
    unindent();
    _length += _sink->write('}');
  }

 private:
  IndentedPrint *_indenter;

  void indent() {
    _indenter->indent();
    _length += _indenter->println();
  }

  void unindent() {
    _length += _indenter->println();
    _indenter->unindent();
  }
};
}
}