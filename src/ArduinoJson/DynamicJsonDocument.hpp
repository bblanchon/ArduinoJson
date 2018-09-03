// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicMemoryPool.hpp"

namespace ArduinoJson {

class DynamicJsonDocument {
 public:
  uint8_t nestingLimit;

  DynamicJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  DynamicJsonDocument(size_t capacity)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT),
        _memoryPool(capacity) {}

  template <typename T>
  bool is() const {
    return getVariant().is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return getVariant().as<T>();
  }

  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to() {
    clear();
    JsonObject object(&_memoryPool);
    getVariant().set(object);
    return object;
  }

  // JsonArray to<JsonArray>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to() {
    clear();
    JsonArray array(&_memoryPool);
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

  Internals::DynamicMemoryPool& memoryPool() {
    return _memoryPool;
  }

  void clear() {
    _memoryPool.clear();
    _rootData.setNull();
  }

  size_t memoryUsage() const {
    return _memoryPool.size();
  }

  template <typename Visitor>
  void visit(Visitor& visitor) const {
    return _rootData.visit(visitor);
  }

 private:
  JsonVariant getVariant() const {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  mutable Internals::DynamicMemoryPool _memoryPool;
  mutable Internals::JsonVariantData _rootData;
};
}  // namespace ArduinoJson
