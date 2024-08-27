// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Json/JsonSerializer.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class PrettyJsonSerializer : public JsonSerializer<TWriter> {
  typedef JsonSerializer<TWriter> base;

 public:
  PrettyJsonSerializer(TWriter writer, const ResourceManager* resources)
      : base(writer, resources), nesting_(0) {}

  size_t visit(const ArrayData& array) {
    auto it = array.createIterator(base::resources_);
    if (!it.done()) {
      base::write("[\r\n");
      nesting_++;
      while (!it.done()) {
        indent();
        it->accept(*this, base::resources_);

        it.next(base::resources_);
        base::write(it.done() ? "\r\n" : ",\r\n");
      }
      nesting_--;
      indent();
      base::write("]");
    } else {
      base::write("[]");
    }
    return this->bytesWritten();
  }

  size_t visit(const ObjectData& object) {
    auto it = object.createIterator(base::resources_);
    if (!it.done()) {
      base::write("{\r\n");
      nesting_++;
      bool isKey = true;
      while (!it.done()) {
        if (isKey)
          indent();
        it->accept(*this, base::resources_);
        it.next(base::resources_);
        if (isKey)
          base::write(": ");
        else
          base::write(it.done() ? "\r\n" : ",\r\n");
        isKey = !isKey;
      }
      nesting_--;
      indent();
      base::write("}");
    } else {
      base::write("{}");
    }
    return this->bytesWritten();
  }

  using base::visit;

 private:
  void indent() {
    for (uint8_t i = 0; i < nesting_; i++)
      base::write(ARDUINOJSON_TAB);
  }

  uint8_t nesting_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Produces JsonDocument to create a prettified JSON document.
// https://arduinojson.org/v7/api/json/serializejsonpretty/
template <typename TDestination>
detail::enable_if_t<!detail::is_pointer<TDestination>::value, size_t>
serializeJsonPretty(JsonVariantConst source, TDestination& destination) {
  using namespace ArduinoJson::detail;
  return serialize<PrettyJsonSerializer>(source, destination);
}

// Produces JsonDocument to create a prettified JSON document.
// https://arduinojson.org/v7/api/json/serializejsonpretty/
inline size_t serializeJsonPretty(JsonVariantConst source, void* buffer,
                                  size_t bufferSize) {
  using namespace ArduinoJson::detail;
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}

// Computes the length of the document that serializeJsonPretty() produces.
// https://arduinojson.org/v7/api/json/measurejsonpretty/
inline size_t measureJsonPretty(JsonVariantConst source) {
  using namespace ArduinoJson::detail;
  return measure<PrettyJsonSerializer>(source);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
