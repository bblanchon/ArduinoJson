// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicMemoryPool.hpp"
#include "Memory/StaticMemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TMemoryPool>
class JsonDocument {
 public:
  uint8_t nestingLimit;

  JsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  template <typename T>
  typename JsonVariantAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename JsonVariantTo<T>::type to() {
    _memoryPool.clear();
    return getVariant().template to<T>();
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

  TMemoryPool& memoryPool() {
    return _memoryPool;
  }

 private:
  JsonVariant getVariant() const {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  mutable TMemoryPool _memoryPool;
  mutable JsonVariantData _rootData;
};

class DynamicJsonDocument : public JsonDocument<DynamicMemoryPool> {
 public:
  DynamicJsonDocument() {}
  DynamicJsonDocument(size_t capacity) {
    memoryPool().reserve(capacity);
  }
};

template <size_t CAPACITY>
class StaticJsonDocument : public JsonDocument<StaticMemoryPool<CAPACITY> > {
 public:
  StaticMemoryPoolBase& memoryPool() {
    return JsonDocument<StaticMemoryPool<CAPACITY> >::memoryPool();
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
