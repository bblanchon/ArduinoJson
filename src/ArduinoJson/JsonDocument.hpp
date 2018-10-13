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
class JsonDocument : public Visitable {
 public:
  uint8_t nestingLimit;

  JsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }

  template <typename T>
  typename JsonVariantAs<T>::type as() {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename JsonVariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  void clear() {
    _memoryPool.clear();
    _rootData.type = JSON_NULL;
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  size_t memoryUsage() const {
    return _memoryPool.size();
  }

  TMemoryPool& memoryPool() {
    return _memoryPool;
  }

  template <typename T>
  typename JsonVariantTo<T>::type to() {
    _memoryPool.clear();
    return getVariant().template to<T>();
  }

 protected:
  template <typename T>
  void copy(const JsonDocument<T>& src) {
    nestingLimit = src.nestingLimit;
    to<JsonVariant>().set(src.template as<JsonVariant>());
  }

 private:
  JsonVariant getVariant() {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  JsonVariantConst getVariant() const {
    return JsonVariantConst(&_rootData);
  }

  TMemoryPool _memoryPool;
  JsonVariantData _rootData;
};

class DynamicJsonDocument : public JsonDocument<DynamicMemoryPool> {
 public:
  DynamicJsonDocument() {}
  DynamicJsonDocument(size_t capacity) {
    memoryPool().reserve(capacity);
  }

  DynamicJsonDocument(const DynamicJsonDocument& src) {
    memoryPool().reserve(src.memoryUsage());
    copy(src);
  }

  template <typename T>
  DynamicJsonDocument(const JsonDocument<T>& src) {
    memoryPool().reserve(src.memoryUsage());
    copy(src);
  }

  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    copy(src);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const JsonDocument<T>& src) {
    copy(src);
    return *this;
  }
};

template <size_t CAPACITY>
class StaticJsonDocument : public JsonDocument<StaticMemoryPool<CAPACITY> > {
 public:
  StaticJsonDocument() {}

  template <typename T>
  StaticJsonDocument(const JsonDocument<T>& src) {
    this->copy(src);
  }

  StaticMemoryPoolBase& memoryPool() {
    return JsonDocument<StaticMemoryPool<CAPACITY> >::memoryPool();
  }

  template <typename T>
  StaticJsonDocument operator=(const JsonDocument<T>& src) {
    this->copy(src);
    return *this;
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
