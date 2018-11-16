// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantTo.hpp"
#include "JsonVariant.hpp"
#include "Memory/MemoryPool.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonDocument : public Visitable {
 public:
  uint8_t nestingLimit;

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

  size_t capacity() const {
    return _memoryPool.capacity();
  }

  // for internal use only
  MemoryPool& memoryPool() {
    return _memoryPool;
  }

  template <typename T>
  typename JsonVariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }

 protected:
  JsonDocument(char* buf, size_t capa)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT),
        _memoryPool(buf, capa) {}

  void copy(const JsonDocument& src) {
    nestingLimit = src.nestingLimit;
    to<JsonVariant>().set(src.as<JsonVariant>());
  }

 private:
  JsonVariant getVariant() {
    return JsonVariant(&_memoryPool, &_rootData);
  }

  JsonVariantConst getVariant() const {
    return JsonVariantConst(&_rootData);
  }

  MemoryPool _memoryPool;
  JsonVariantData _rootData;
};

}  // namespace ARDUINOJSON_NAMESPACE
