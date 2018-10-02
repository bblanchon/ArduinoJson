// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/StaticMemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <size_t CAPACITY>
class StaticJsonDocument {
 public:
  uint8_t nestingLimit;

  StaticJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  StaticMemoryPoolBase& memoryPool() {
    return _memoryPool;
  }

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

 private:
  JsonVariant getVariant() const {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  mutable StaticMemoryPool<CAPACITY> _memoryPool;
  mutable JsonVariantData _rootData;
};

}  // namespace ARDUINOJSON_NAMESPACE
