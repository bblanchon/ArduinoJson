// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Serialization/measure.hpp"
#include "../Serialization/serialize.hpp"
#include "JsonSerializer.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class PrettyJsonSerializer : public JsonSerializer<TWriter> {
  typedef JsonSerializer<TWriter> base;

 public:
  PrettyJsonSerializer(TWriter &writer) : base(writer), _nesting(0) {}

  void visitArray(const CollectionData &array) {
    VariantSlot *slot = array.head();
    if (!slot) return base::write("[]");

    base::write("[\r\n");
    _nesting++;
    while (slot != 0) {
      indent();
      slot->data()->accept(*this);

      slot = slot->next();
      base::write(slot ? ",\r\n" : "\r\n");
    }
    _nesting--;
    indent();
    base::write("]");
  }

  void visitObject(const CollectionData &object) {
    VariantSlot *slot = object.head();
    if (!slot) return base::write("{}");

    base::write("{\r\n");
    _nesting++;
    while (slot != 0) {
      indent();
      base::visitString(slot->key());
      base::write(": ");
      slot->data()->accept(*this);

      slot = slot->next();
      base::write(slot ? ",\r\n" : "\r\n");
    }
    _nesting--;
    indent();
    base::write("}");
  }

 private:
  void indent() {
    for (uint8_t i = 0; i < _nesting; i++) base::write(ARDUINOJSON_TAB);
  }

  uint8_t _nesting;
};

template <typename TSource, typename TDestination>
size_t serializeJsonPretty(const TSource &source, TDestination &destination) {
  return serialize<PrettyJsonSerializer>(source, destination);
}

template <typename TSource>
size_t serializeJsonPretty(const TSource &source, char *buffer,
                           size_t bufferSize) {
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}

template <typename TSource>
size_t measureJsonPretty(const TSource &source) {
  return measure<PrettyJsonSerializer>(source);
}

}  // namespace ARDUINOJSON_NAMESPACE
