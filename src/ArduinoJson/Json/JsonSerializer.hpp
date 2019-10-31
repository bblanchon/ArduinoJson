// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class JsonSerializer {
 public:
  JsonSerializer(TWriter writer) : _formatter(writer) {}

  FORCE_INLINE void visitArray(const CollectionData &array) {
    write('[');

    VariantSlot *slot = array.head();

    while (slot != 0) {
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0) break;

      write(',');
    }

    write(']');
  }

  void visitObject(const CollectionData &object) {
    write('{');

    VariantSlot *slot = object.head();

    while (slot != 0) {
      _formatter.writeString(slot->key());
      write(':');
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0) break;

      write(',');
    }

    write('}');
  }

  void visitFloat(Float value) {
    _formatter.writeFloat(value);
  }

  void visitString(const char *value) {
    _formatter.writeString(value);
  }

  void visitRawJson(const char *data, size_t n) {
    _formatter.writeRaw(data, n);
  }

  void visitNegativeInteger(UInt value) {
    _formatter.writeNegativeInteger(value);
  }

  void visitPositiveInteger(UInt value) {
    _formatter.writePositiveInteger(value);
  }

  void visitBoolean(bool value) {
    _formatter.writeBoolean(value);
  }

  void visitNull() {
    _formatter.writeRaw("null");
  }

  size_t bytesWritten() const {
    return _formatter.bytesWritten();
  }

 protected:
  void write(char c) {
    _formatter.writeRaw(c);
  }

  void write(const char *s) {
    _formatter.writeRaw(s);
  }

 private:
  TextFormatter<TWriter> _formatter;
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
