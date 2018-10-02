// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicMemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

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
  typename JsonVariantAs<T>::type as() const {
    return getVariant().as<T>();
  }

  template <typename T>
  typename JsonVariantTo<T>::type to() {
    _memoryPool.clear();
    return getVariant().to<T>();
  }

  void clear() {
    _memoryPool.clear();
    _rootData.type = JSON_NULL;
  }

  size_t memoryUsage() const {
    return _memoryPool.size();
  }

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }

  DynamicMemoryPool& memoryPool() {
    return _memoryPool;
  }

 private:
  JsonVariant getVariant() const {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  mutable DynamicMemoryPool _memoryPool;
  mutable JsonVariantData _rootData;
};
}  // namespace ARDUINOJSON_NAMESPACE
