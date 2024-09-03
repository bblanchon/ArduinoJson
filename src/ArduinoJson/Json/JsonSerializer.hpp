// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>
#include <ArduinoJson/Variant/VariantDataVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class JsonSerializer : public VariantDataVisitor<size_t> {
 public:
  static const bool producesText = true;

  JsonSerializer(TWriter writer, const ResourceManager* resources)
      : formatter_(writer), resources_(resources) {}

  size_t visit(const ArrayData& array) {
    write('[');

    auto slotId = array.head();

    while (slotId != NULL_SLOT) {
      auto slot = resources_->getVariant(slotId);

      slot->accept(*this, resources_);

      slotId = slot->next();

      if (slotId != NULL_SLOT)
        write(',');
    }

    write(']');
    return bytesWritten();
  }

  size_t visit(const ObjectData& object) {
    write('{');

    auto slotId = object.head();

    bool isKey = true;

    while (slotId != NULL_SLOT) {
      auto slot = resources_->getVariant(slotId);
      slot->accept(*this, resources_);

      slotId = slot->next();

      if (slotId != NULL_SLOT)
        write(isKey ? ':' : ',');

      isKey = !isKey;
    }

    write('}');
    return bytesWritten();
  }

  template <typename T>
  enable_if_t<is_floating_point<T>::value, size_t> visit(T value) {
    formatter_.writeFloat(value);
    return bytesWritten();
  }

  size_t visit(const char* value) {
    formatter_.writeString(value);
    return bytesWritten();
  }

  size_t visit(JsonString value) {
    formatter_.writeString(value.c_str(), value.size());
    return bytesWritten();
  }

  size_t visit(RawString value) {
    formatter_.writeRaw(value.data(), value.size());
    return bytesWritten();
  }

  size_t visit(JsonInteger value) {
    formatter_.writeInteger(value);
    return bytesWritten();
  }

  size_t visit(JsonUInt value) {
    formatter_.writeInteger(value);
    return bytesWritten();
  }

  size_t visit(bool value) {
    formatter_.writeBoolean(value);
    return bytesWritten();
  }

  size_t visit(nullptr_t) {
    formatter_.writeRaw("null");
    return bytesWritten();
  }

 protected:
  size_t bytesWritten() const {
    return formatter_.bytesWritten();
  }

  void write(char c) {
    formatter_.writeRaw(c);
  }

  void write(const char* s) {
    formatter_.writeRaw(s);
  }

 private:
  TextFormatter<TWriter> formatter_;

 protected:
  const ResourceManager* resources_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Produces a minified JSON document.
// https://arduinojson.org/v7/api/json/serializejson/
template <typename TDestination>
detail::enable_if_t<!detail::is_pointer<TDestination>::value, size_t>
serializeJson(JsonVariantConst source, TDestination& destination) {
  using namespace detail;
  return serialize<JsonSerializer>(source, destination);
}

// Produces a minified JSON document.
// https://arduinojson.org/v7/api/json/serializejson/
inline size_t serializeJson(JsonVariantConst source, void* buffer,
                            size_t bufferSize) {
  using namespace detail;
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}

// Computes the length of the document that serializeJson() produces.
// https://arduinojson.org/v7/api/json/measurejson/
inline size_t measureJson(JsonVariantConst source) {
  using namespace detail;
  return measure<JsonSerializer>(source);
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline detail::enable_if_t<detail::is_convertible<T, JsonVariantConst>::value,
                           std::ostream&>
operator<<(std::ostream& os, const T& source) {
  serializeJson(source, os);
  return os;
}
#endif

ARDUINOJSON_END_PUBLIC_NAMESPACE
