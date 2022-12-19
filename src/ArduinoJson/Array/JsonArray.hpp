// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/ElementProxy.hpp>
#include <ArduinoJson/Array/JsonArrayConst.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonObject;

class JsonArray : public VariantOperators<JsonArray> {
  friend class VariantAttorney;

 public:
  typedef JsonArrayIterator iterator;

  FORCE_INLINE JsonArray() : _data(0), _pool(0) {}
  FORCE_INLINE JsonArray(MemoryPool* pool, CollectionData* data)
      : _data(data), _pool(pool) {}

  operator VariantRef() {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }

  operator JsonArrayConst() const {
    return JsonArrayConst(_data);
  }

  VariantRef add() const {
    if (!_data)
      return VariantRef();
    return VariantRef(_pool, _data->addElement(_pool));
  }

  template <typename T>
  FORCE_INLINE bool add(const T& value) const {
    return add().set(value);
  }

  template <typename T>
  FORCE_INLINE bool add(T* value) const {
    return add().set(value);
  }

  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_pool, _data->head());
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Copy a JsonArray
  FORCE_INLINE bool set(JsonArrayConst src) const {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }

  FORCE_INLINE bool operator==(JsonArray rhs) const {
    return JsonArrayConst(_data) == JsonArrayConst(rhs._data);
  }

  // Removes element at specified position.
  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it._slot);
  }

  // Removes element at specified index.
  FORCE_INLINE void remove(size_t index) const {
    if (!_data)
      return;
    _data->removeElement(index);
  }

  void clear() const {
    if (!_data)
      return;
    _data->clear();
  }

  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE ElementProxy<JsonArray> operator[](size_t index) const {
    return ElementProxy<JsonArray>(*this, index);
  }

  FORCE_INLINE JsonObject createNestedObject() const;

  FORCE_INLINE JsonArray createNestedArray() const {
    return add().to<JsonArray>();
  }

  operator JsonVariantConst() const {
    return JsonVariantConst(collectionToVariant(_data));
  }

  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  FORCE_INLINE operator bool() const {
    return _data != 0;
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return variantNesting(collectionToVariant(_data));
  }

  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }

 private:
  MemoryPool* getPool() const {
    return _pool;
  }

  VariantData* getData() const {
    return collectionToVariant(_data);
  }

  VariantData* getOrCreateData() const {
    return collectionToVariant(_data);
  }

  CollectionData* _data;
  MemoryPool* _pool;
};

template <>
struct Converter<JsonArray> : private VariantAttorney {
  static void toJson(JsonVariantConst src, VariantRef dst) {
    variantCopyFrom(getData(dst), getData(src), getPool(dst));
  }

  static JsonArray fromJson(VariantRef src) {
    VariantData* data = getData(src);
    MemoryPool* pool = getPool(src);
    return JsonArray(pool, data != 0 ? data->asArray() : 0);
  }

  static InvalidConversion<JsonVariantConst, JsonArray> fromJson(
      JsonVariantConst);

  static bool checkJson(JsonVariantConst) {
    return false;
  }

  static bool checkJson(VariantRef src) {
    VariantData* data = getData(src);
    return data && data->isArray();
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
