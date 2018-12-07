// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Variant/VariantRef.hpp"
#include "../Variant/VariantTo.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonDocument : public Visitable {
 public:
  uint8_t nestingLimit;

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return getVariant().accept(visitor);
  }

  template <typename T>
  typename VariantAs<T>::type as() {
    return getVariant().template as<T>();
  }

  template <typename T>
  typename VariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }

  void clear() {
    _pool.clear();
    _data.setNull();
  }

  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }

  size_t memoryUsage() const {
    return _pool.size();
  }

  size_t capacity() const {
    return _pool.capacity();
  }

  template <typename T>
  typename VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }

  // for internal use only
  MemoryPool& memoryPool() {
    return _pool;
  }

  VariantData& data() {
    return _data;
  }

 protected:
  JsonDocument(MemoryPool pool)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT), _pool(pool) {}

  JsonDocument(char* buf, size_t capa)
      : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT), _pool(buf, capa) {}

  void copy(const JsonDocument& src) {
    nestingLimit = src.nestingLimit;
    to<VariantRef>().set(src.as<VariantRef>());
  }

  void replacePool(MemoryPool pool) {
    _pool = pool;
  }

 private:
  VariantRef getVariant() {
    return VariantRef(&_pool, &_data);
  }

  VariantConstRef getVariant() const {
    return VariantConstRef(&_data);
  }

  MemoryPool _pool;
  VariantData _data;
};

}  // namespace ARDUINOJSON_NAMESPACE
