// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Serialization/measure.hpp"
#include "../Serialization/serialize.hpp"
#include "./JsonWriter.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TWriter>
class JsonSerializer {
 public:
  JsonSerializer(TWriter &writer) : _writer(writer) {}

  void acceptFloat(JsonFloat value) {
    _writer.writeFloat(value);
  }

  void acceptArray(const JsonArray &array) {
    _writer.beginArray();

    JsonArray::const_iterator it = array.begin();
    while (it != array.end()) {
      it->visit(*this);

      ++it;
      if (it == array.end()) break;

      _writer.writeComma();
    }

    _writer.endArray();
  }

  void acceptObject(const JsonObject &object) {
    _writer.beginObject();

    JsonObject::const_iterator it = object.begin();
    while (it != object.end()) {
      _writer.writeString(it->key);
      _writer.writeColon();
      it->value.visit(*this);

      ++it;
      if (it == object.end()) break;

      _writer.writeComma();
    }

    _writer.endObject();
  }

  void acceptString(const char *value) {
    _writer.writeString(value);
  }

  void acceptRawJson(const char *data, size_t n) {
    // TODO
    for (size_t i = 0; i < n; i++) _writer.writeRaw(data[i]);
  }

  void acceptNegativeInteger(JsonUInt value) {
    _writer.writeRaw('-');
    _writer.writeInteger(value);
  }

  void acceptPositiveInteger(JsonUInt value) {
    _writer.writeInteger(value);
  }

  void acceptBoolean(bool value) {
    _writer.writeBoolean(value);
  }

  void acceptNull() {
    _writer.writeRaw("null");
  }

  size_t bytesWritten() const {
    return _writer.bytesWritten();
  }

 private:
  JsonWriter<TWriter> _writer;
};

}  // namespace Internals

template <typename TSource, typename TDestination>
size_t serializeJson(TSource &source, TDestination &destination) {
  using namespace Internals;
  return serialize<JsonSerializer>(source, destination);
}

template <typename TSource>
size_t serializeJson(const TSource &source, char *buffer, size_t bufferSize) {
  using namespace Internals;
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}

template <typename TSource>
size_t measureJson(const TSource &source) {
  using namespace Internals;
  return measure<JsonSerializer>(source);
}

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream &operator<<(std::ostream &os, const JsonArray &source) {
  serializeJson(source, os);
  return os;
}
inline std::ostream &operator<<(std::ostream &os, const JsonObject &source) {
  serializeJson(source, os);
  return os;
}
inline std::ostream &operator<<(std::ostream &os, const JsonVariant &source) {
  serializeJson(source, os);
  return os;
}

namespace Internals {
inline std::ostream &operator<<(std::ostream &os,
                                const JsonArraySubscript &source) {
  serializeJson(source, os);
  return os;
}

template <typename TKey>
inline std::ostream &operator<<(std::ostream &os,
                                const JsonObjectSubscript<TKey> &source) {
  serializeJson(source, os);
  return os;
}
}  // namespace Internals

#endif

}  // namespace ArduinoJson
