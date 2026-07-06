// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Json/JsonSerializer.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class PrettyJsonSerializer : public JsonSerializer<TWriter> {
  using base = JsonSerializer<TWriter>;

 public:
  PrettyJsonSerializer(TWriter writer, ResourceManager* resources)
      : base(writer, resources), nesting_(0) {}

  size_t visitArray(VariantData* array) {
    ARDUINOJSON_ASSERT(array != nullptr);
    ARDUINOJSON_ASSERT(array->isArray());

    auto slotId = array->content.asCollection.head;
    if (slotId != NULL_SLOT) {
      base::write("[\r\n");
      nesting_++;
      while (slotId != NULL_SLOT) {
        indent();
        auto slot = base::resources_->getVariant(slotId);
        VariantImpl::accept(*this, slot, base::resources_);

        slotId = slot->next;
        base::write(slotId == NULL_SLOT ? "\r\n" : ",\r\n");
      }
      nesting_--;
      indent();
      base::write("]");
    } else {
      base::write("[]");
    }
    return this->bytesWritten();
  }

  size_t visitObject(VariantData* object) {
    ARDUINOJSON_ASSERT(object != nullptr);
    ARDUINOJSON_ASSERT(object->isObject());

    auto slotId = object->content.asCollection.head;
    if (slotId != NULL_SLOT) {
      base::write("{\r\n");
      nesting_++;
      bool isKey = true;
      while (slotId != NULL_SLOT) {
        if (isKey)
          indent();
        auto slot = base::resources_->getVariant(slotId);
        VariantImpl::accept(*this, slot, base::resources_);
        slotId = slot->next;
        if (isKey)
          base::write(": ");
        else
          base::write(slotId == NULL_SLOT ? "\r\n" : ",\r\n");
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
template <
    typename TDestination,
    detail::enable_if_t<!detail::is_pointer<TDestination>::value, int> = 0>
inline size_t serializeJsonPretty(JsonVariantConst source,
                                  TDestination& destination) {
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
