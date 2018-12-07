// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Misc/Visitable.hpp"
#include "../Serialization/measure.hpp"
#include "../Serialization/serialize.hpp"
#include "JsonWriter.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class JsonSerializer {
 public:
  JsonSerializer(TWriter &writer) : _writer(writer) {}

  void visitFloat(Float value) {
    _writer.writeFloat(value);
  }

  void visitArray(const CollectionData &array) {
    _writer.beginArray();

    VariantSlot *slot = array.head();

    while (slot != 0) {
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0) break;

      _writer.writeComma();
    }

    _writer.endArray();
  }

  void visitObject(const CollectionData &object) {
    _writer.beginObject();

    VariantSlot *slot = object.head();

    while (slot != 0) {
      _writer.writeString(slot->key());
      _writer.writeColon();
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0) break;

      _writer.writeComma();
    }

    _writer.endObject();
  }

  void visitString(const char *value) {
    _writer.writeString(value);
  }

  void visitRawJson(const char *data, size_t n) {
    // TODO
    for (size_t i = 0; i < n; i++) _writer.writeRaw(data[i]);
  }

  void visitNegativeInteger(UInt value) {
    _writer.writeRaw('-');
    _writer.writeInteger(value);
  }

  void visitPositiveInteger(UInt value) {
    _writer.writeInteger(value);
  }

  void visitBoolean(bool value) {
    _writer.writeBoolean(value);
  }

  void visitNull() {
    _writer.writeRaw("null");
  }

  size_t bytesWritten() const {
    return _writer.bytesWritten();
  }

 private:
  JsonWriter<TWriter> _writer;
};

template <typename TSource, typename TDestination>
size_t serializeJson(const TSource &source, TDestination &destination) {
  return serialize<JsonSerializer>(source, destination);
}

template <typename TSource>
size_t serializeJson(const TSource &source, char *buffer, size_t bufferSize) {
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}

template <typename TSource>
size_t measureJson(const TSource &source) {
  return measure<JsonSerializer>(source);
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline typename enable_if<IsVisitable<T>::value, std::ostream &>::type
operator<<(std::ostream &os, const T &source) {
  serializeJson(source, os);
  return os;
}
#endif

}  // namespace ARDUINOJSON_NAMESPACE
