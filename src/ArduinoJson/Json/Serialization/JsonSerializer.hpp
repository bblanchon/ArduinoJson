// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../../Print/DummyPrint.hpp"
#include "../../Print/DynamicStringBuilder.hpp"
#include "../../Print/StaticStringBuilder.hpp"
#include "./IndentedPrint.hpp"
#include "./JsonWriter.hpp"
#include "./Prettyfier.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "../../Print/StreamPrintAdapter.hpp"
#endif

namespace ArduinoJson {
namespace Internals {

template <typename Writer>
class JsonSerializer {
 public:
  template <typename TSource>
  static void serialize(const TSource &source, Writer &writer) {
    source.visit(Visitor(&writer));
  }

  struct Visitor {
    Visitor(Writer *writer) : _writer(writer) {}

    void acceptFloat(JsonFloat value) {
      _writer->writeFloat(value);
    }

    void acceptArray(const JsonArray &array) {
      _writer->beginArray();

      JsonArray::const_iterator it = array.begin();
      while (it != array.end()) {
        it->visit(*this);

        ++it;
        if (it == array.end()) break;

        _writer->writeComma();
      }

      _writer->endArray();
    }

    void acceptObject(const JsonObject &object) {
      _writer->beginObject();

      JsonObject::const_iterator it = object.begin();
      while (it != object.end()) {
        _writer->writeString(it->key);
        _writer->writeColon();
        it->value.visit(*this);

        ++it;
        if (it == object.end()) break;

        _writer->writeComma();
      }

      _writer->endObject();
    }

    void acceptString(const char *value) {
      _writer->writeString(value);
    }

    void acceptRawJson(const char *value) {
      _writer->writeRaw(value);
    }

    void acceptNegativeInteger(JsonUInt value) {
      _writer->writeRaw('-');
      _writer->writeInteger(value);
    }

    void acceptPositiveInteger(JsonUInt value) {
      _writer->writeInteger(value);
    }

    void acceptBoolean(bool value) {
      _writer->writeBoolean(value);
    }

    void acceptUndefined() {}

    Writer *_writer;
  };
};
}  // namespace Internals

template <typename TSource, typename TDestination>
typename Internals::EnableIf<!Internals::StringTraits<TDestination>::has_append,
                             size_t>::type
serializeJson(const TSource &source, TDestination &destination) {
  Internals::JsonWriter<TDestination> writer(destination);
  Internals::JsonSerializer<Internals::JsonWriter<TDestination> >::serialize(
      source, writer);
  return writer.bytesWritten();
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename TSource>
std::ostream &serializeJson(const TSource &source, std::ostream &os) {
  Internals::StreamPrintAdapter adapter(os);
  serializeJson(source, adapter);
  return os;
}
#endif

template <typename TSource>
size_t serializeJson(const TSource &source, char *buffer, size_t bufferSize) {
  Internals::StaticStringBuilder sb(buffer, bufferSize);
  return serializeJson(source, sb);
}

template <typename TSource, size_t N>
size_t serializeJson(const TSource &source, char (&buffer)[N]) {
  return serializeJson(source, buffer, N);
}

template <typename TSource, typename TDestination>
typename Internals::EnableIf<Internals::StringTraits<TDestination>::has_append,
                             size_t>::type
serializeJson(const TSource &source, TDestination &str) {
  Internals::DynamicStringBuilder<TDestination> sb(str);
  return serializeJson(source, sb);
}

template <typename TSource, typename TDestination>
size_t serializeJsonPretty(const TSource &source,
                           Internals::IndentedPrint<TDestination> &print) {
  Internals::Prettyfier<TDestination> p(print);
  return serializeJson(source, p);
}

template <typename TSource>
size_t serializeJsonPretty(const TSource &source, char *buffer,
                           size_t bufferSize) {
  Internals::StaticStringBuilder sb(buffer, bufferSize);
  return serializeJsonPretty(source, sb);
}

template <typename TSource, size_t N>
size_t serializeJsonPretty(const TSource &source, char (&buffer)[N]) {
  return serializeJsonPretty(source, buffer, N);
}

template <typename TSource, typename TDestination>
typename Internals::EnableIf<!Internals::StringTraits<TDestination>::has_append,
                             size_t>::type
serializeJsonPretty(const TSource &source, TDestination &print) {
  Internals::IndentedPrint<TDestination> indentedPrint(print);
  return serializeJsonPretty(source, indentedPrint);
}

template <typename TSource, typename TDestination>
typename Internals::EnableIf<Internals::StringTraits<TDestination>::has_append,
                             size_t>::type
serializeJsonPretty(const TSource &source, TDestination &str) {
  Internals::DynamicStringBuilder<TDestination> sb(str);
  return serializeJsonPretty(source, sb);
}

template <typename TSource>
size_t measureJson(const TSource &source) {
  Internals::DummyPrint dp;
  return serializeJson(source, dp);
}

template <typename TSource>
size_t measureJsonPretty(const TSource &source) {
  Internals::DummyPrint dp;
  return serializeJsonPretty(source, dp);
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
