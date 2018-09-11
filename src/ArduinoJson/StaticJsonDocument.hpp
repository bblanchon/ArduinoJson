// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/StaticMemoryPool.hpp"

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonDocument {
 public:
  uint8_t nestingLimit;

  StaticJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  Internals::StaticMemoryPoolBase& memoryPool() {
    return _memoryPool;
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename Internals::JsonVariantTo<T>::type to() {
    _memoryPool.clear();
    return getVariant().template to<T>();
  }

  void clear() {
    _memoryPool.clear();
    _rootData.setNull();
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

  mutable Internals::StaticMemoryPool<CAPACITY> _memoryPool;
  mutable Internals::JsonVariantData _rootData;
};

}  // namespace ArduinoJson
