// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {

class DynamicJsonDocument {
  Internals::DynamicJsonBuffer _buffer;
  JsonVariant _root;

 public:
  DynamicJsonDocument() {}
  DynamicJsonDocument(size_t capacity) : _buffer(capacity) {}

  template <typename T>
  bool is() const {
    return _root.is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return _root.as<T>();
  }

  // JsonObject& to<JsonObject>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonObject>::value,
                               JsonObject&>::type
  to() {
    clear();
    JsonObject* object = new (&_buffer) JsonObject(&_buffer);
    if (!object) return JsonObject::invalid();
    _root = object;
    return *object;
  }

  // JsonArray& to<JsonArray>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonArray>::value,
                               JsonArray&>::type
  to() {
    clear();
    JsonArray* array = new (&_buffer) JsonArray(&_buffer);
    if (!array) return JsonArray::invalid();
    _root = array;
    return *array;
  }

  // JsonVariant& to<JsonVariant>()
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonVariant>::value,
                               T&>::type
  to() {
    clear();
    return _root;
  }

  Internals::DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  void clear() {
    _buffer.clear();
    _root = JsonVariant();
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }

  template <typename Visitor>
  void visit(Visitor visitor) const {
    return _root.visit(visitor);
  }
};
}  // namespace ArduinoJson
