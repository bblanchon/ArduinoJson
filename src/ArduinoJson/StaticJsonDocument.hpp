// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "Memory/StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonDocument {
 public:
  uint8_t nestingLimit;

  StaticJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  Internals::StaticJsonBufferBase& buffer() {
    return _buffer;
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to() {
    clear();
    JsonObject object(&_buffer);
    getVariant().set(object);
    return object;
  }

  // JsonArray to<JsonArray>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to() {
    clear();
    JsonArray array(&_buffer);
    getVariant().set(array);
    return array;
  }

  // JsonVariant to<JsonVariant>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                JsonVariant>::type
  to() {
    clear();
    return getVariant();
  }

  // JsonVariantData& to<JsonVariantData>()
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<T, Internals::JsonVariantData>::value,
      Internals::JsonVariantData&>::type
  to() {
    clear();
    return _rootData;
  }

  void clear() {
    _buffer.clear();
    _rootData.setNull();
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }

  template <typename Visitor>
  void visit(Visitor& visitor) const {
    return getVariant().visit(visitor);
  }

 private:
  JsonVariant getVariant() const {
    return JsonVariant(&_buffer, &_rootData);
  }

  mutable Internals::StaticJsonBuffer<CAPACITY> _buffer;
  mutable Internals::JsonVariantData _rootData;
};

}  // namespace ArduinoJson
