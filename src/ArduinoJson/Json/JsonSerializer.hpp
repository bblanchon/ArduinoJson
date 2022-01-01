// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>
#include <ArduinoJson/Variant/Visitor.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class JsonSerializer : public Visitor<size_t> {
 public:
  static const bool producesText = true;

  JsonSerializer(TWriter writer) : _formatter(writer) {}

  FORCE_INLINE size_t visitArray(const CollectionData &array) {
    write('[');

    VariantSlot *slot = array.head();

    while (slot != 0) {
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0)
        break;

      write(',');
    }

    write(']');
    return bytesWritten();
  }

  size_t visitObject(const CollectionData &object) {
    write('{');

    VariantSlot *slot = object.head();

    while (slot != 0) {
      _formatter.writeString(slot->key());
      write(':');
      slot->data()->accept(*this);

      slot = slot->next();
      if (slot == 0)
        break;

      write(',');
    }

    write('}');
    return bytesWritten();
  }

  size_t visitFloat(Float value) {
    _formatter.writeFloat(value);
    return bytesWritten();
  }

  size_t visitString(const char *value) {
    _formatter.writeString(value);
    return bytesWritten();
  }

  size_t visitString(const char *value, size_t n) {
    _formatter.writeString(value, n);
    return bytesWritten();
  }

  size_t visitRawJson(const char *data, size_t n) {
    _formatter.writeRaw(data, n);
    return bytesWritten();
  }

  size_t visitSignedInteger(Integer value) {
    _formatter.writeInteger(value);
    return bytesWritten();
  }

  size_t visitUnsignedInteger(UInt value) {
    _formatter.writeInteger(value);
    return bytesWritten();
  }

  size_t visitBoolean(bool value) {
    _formatter.writeBoolean(value);
    return bytesWritten();
  }

  size_t visitNull() {
    _formatter.writeRaw("null");
    return bytesWritten();
  }

 protected:
  size_t bytesWritten() const {
    return _formatter.bytesWritten();
  }

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
size_t serializeJson(const TSource &source, void *buffer, size_t bufferSize) {
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
