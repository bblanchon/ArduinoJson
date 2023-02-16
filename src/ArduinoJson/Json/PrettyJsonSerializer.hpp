// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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
  PrettyJsonSerializer(TWriter writer) : base(writer), _nesting(0) {}

  size_t visitArray(const CollectionData& array) {
    const VariantSlot* slot = array.head();
    if (slot) {
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
    } else {
      base::write("[]");
    }
    return this->bytesWritten();
  }

  size_t visitObject(const CollectionData& object) {
    const VariantSlot* slot = object.head();
    if (slot) {
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
    } else {
      base::write("{}");
    }
    return this->bytesWritten();
  }

 private:
  void indent() {
    for (uint8_t i = 0; i < _nesting; i++)
      base::write(ARDUINOJSON_TAB);
  }

  uint8_t _nesting;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Produces JsonDocument to create a prettified JSON document.
// https://arduinojson.org/v6/api/json/serializejsonpretty/
template <typename TDestination>
size_t serializeJsonPretty(JsonVariantConst source, TDestination& destination) {
  using namespace ArduinoJson::detail;
  return serialize<PrettyJsonSerializer>(source, destination);
}

// Produces JsonDocument to create a prettified JSON document.
// https://arduinojson.org/v6/api/json/serializejsonpretty/
inline size_t serializeJsonPretty(JsonVariantConst source, void* buffer,
                                  size_t bufferSize) {
  using namespace ArduinoJson::detail;
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}

// Computes the length of the document that serializeJsonPretty() produces.
// https://arduinojson.org/v6/api/json/measurejsonpretty/
inline size_t measureJsonPretty(JsonVariantConst source) {
  using namespace ArduinoJson::detail;
  return measure<PrettyJsonSerializer>(source);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
