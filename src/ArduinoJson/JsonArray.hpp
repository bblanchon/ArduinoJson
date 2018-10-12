// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/ArrayFunctions.hpp"
#include "Data/JsonVariantData.hpp"
#include "JsonArrayIterator.hpp"

// Returns the size (in bytes) of an array with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::Slot))

namespace ARDUINOJSON_NAMESPACE {

class JsonObject;
class JsonArraySubscript;

template <typename TData>
class JsonArrayProxy {
 public:
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }

  FORCE_INLINE JsonVariantConst operator[](size_t index) const {
    return JsonVariantConst(arrayGet(_data, index));
  }

  FORCE_INLINE size_t size() const {
    return arraySize(_data);
  }

 protected:
  JsonArrayProxy(TData* data) : _data(data) {}
  TData* _data;
};

class JsonArrayConst : public JsonArrayProxy<const JsonArrayData>,
                       public Visitable {
  friend class JsonArray;
  typedef JsonArrayProxy<const JsonArrayData> proxy_type;

 public:
  typedef JsonArrayConstIterator iterator;

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    if (_data)
      visitor.visitArray(*this);
    else
      visitor.visitNull();
  }

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_data->head);
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  FORCE_INLINE JsonArrayConst() : proxy_type(0) {}
  FORCE_INLINE JsonArrayConst(const JsonArrayData* data) : proxy_type(data) {}

  FORCE_INLINE bool operator==(JsonArrayConst rhs) const {
    return arrayEquals(_data, rhs._data);
  }
};

class JsonArray : public JsonArrayProxy<JsonArrayData>, public Visitable {
  typedef JsonArrayProxy<JsonArrayData> proxy_type;

 public:
  typedef JsonArrayIterator iterator;

  FORCE_INLINE JsonArray() : proxy_type(0), _memoryPool(0) {}
  FORCE_INLINE JsonArray(MemoryPool* pool, JsonArrayData* data)
      : proxy_type(data), _memoryPool(pool) {}

  operator JsonVariant() {
    return JsonVariant(_memoryPool, getVariantData(_data));
  }

  operator JsonArrayConst() const {
    return JsonArrayConst(_data);
  }

  // Adds the specified value at the end of the array.
  //
  // bool add(TValue);
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonObject
  template <typename T>
  FORCE_INLINE bool add(const T& value) const {
    return add().set(value);
  }
  // Adds the specified value at the end of the array.
  FORCE_INLINE bool add(JsonArray value) const {
    return add().set(value);
  }
  //
  // bool add(TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename T>
  FORCE_INLINE bool add(T* value) const {
    return add().set(value);
  }

  JsonVariant add() const {
    return JsonVariant(_memoryPool, arrayAdd(_data, _memoryPool));
  }

  FORCE_INLINE iterator begin() const {
    if (!_data) return iterator();
    return iterator(_memoryPool, _data->head);
  }

  FORCE_INLINE iterator end() const {
    return iterator();
  }

  // Imports a 1D array
  template <typename T, size_t N>
  FORCE_INLINE bool copyFrom(T (&array)[N]) const {
    return copyFrom(array, N);
  }

  // Imports a 1D array
  template <typename T>
  bool copyFrom(T* array, size_t len) const {
    bool ok = true;
    for (size_t i = 0; i < len; i++) {
      ok &= add(array[i]);
    }
    return ok;
  }

  // Imports a 2D array
  template <typename T, size_t N1, size_t N2>
  bool copyFrom(T (&array)[N1][N2]) const {
    bool ok = true;
    for (size_t i = 0; i < N1; i++) {
      JsonArray nestedArray = createNestedArray();
      for (size_t j = 0; j < N2; j++) {
        ok &= nestedArray.add(array[i][j]);
      }
    }
    return ok;
  }

  // Copy a JsonArray
  FORCE_INLINE bool copyFrom(JsonArray src) const {
    return arrayCopy(_data, src._data, _memoryPool);
  }

  // Exports a 1D array
  template <typename T, size_t N>
  FORCE_INLINE size_t copyTo(T (&array)[N]) const {
    return copyTo(array, N);
  }

  // Exports a 1D array
  template <typename T>
  size_t copyTo(T* array, size_t len) const {
    size_t i = 0;
    for (iterator it = begin(); it != end() && i < len; ++it) array[i++] = *it;
    return i;
  }

  // Exports a 2D array
  template <typename T, size_t N1, size_t N2>
  void copyTo(T (&array)[N1][N2]) const {
    if (!_data) return;
    size_t i = 0;
    for (iterator it = begin(); it != end() && i < N1; ++it) {
      it->as<JsonArray>().copyTo(array[i++]);
    }
  }

  FORCE_INLINE JsonArray createNestedArray() const;
  FORCE_INLINE JsonObject createNestedObject() const;

  FORCE_INLINE JsonArraySubscript operator[](size_t index) const;

  FORCE_INLINE bool operator==(JsonArray rhs) const {
    return arrayEquals(_data, rhs._data);
  }

  // Gets the value at the specified index.
  template <typename T>
  FORCE_INLINE typename JsonVariantAs<T>::type get(size_t index) const {
    return get_impl(index).as<T>();
  }

  // Check the type of the value at specified index.
  template <typename T>
  FORCE_INLINE bool is(size_t index) const {
    return get_impl(index).is<T>();
  }

  // Removes element at specified position.
  FORCE_INLINE void remove(iterator it) const {
    arrayRemove(_data, it.internal());
  }

  // Removes element at specified index.
  FORCE_INLINE void remove(size_t index) const {
    arrayRemove(_data, index);
  }

  // Sets the value at specified index.
  //
  // bool add(size_t index, const TValue&);
  // TValue = bool, long, int, short, float, double, serialized, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename T>
  FORCE_INLINE bool set(size_t index, const T& value) const {
    if (!_data) return false;
    return get_impl(index).set(value);
  }
  //
  // bool add(size_t index, TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename T>
  FORCE_INLINE bool set(size_t index, T* value) const {
    if (!_data) return false;
    return get_impl(index).set(value);
  }

  template <typename Visitor>
  FORCE_INLINE void accept(Visitor& visitor) const {
    JsonArrayConst(_data).accept(visitor);
  }

 private:
  template <typename TValueRef>
  FORCE_INLINE bool add_impl(TValueRef value) const {
    return add().set(value);
  }

  FORCE_INLINE JsonVariant get_impl(size_t index) const {
    return JsonVariant(_memoryPool, arrayGet(_data, index));
  }

  MemoryPool* _memoryPool;
};
}  // namespace ARDUINOJSON_NAMESPACE
