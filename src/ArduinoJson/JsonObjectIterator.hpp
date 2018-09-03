// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/ListIterator.hpp"
#include "JsonPair.hpp"

namespace ArduinoJson {

class JsonPairPtr {
 public:
  JsonPairPtr(Internals::MemoryPool *memoryPool, Internals::JsonPairData *data)
      : _pair(memoryPool, data) {}

  const JsonPair *operator->() const {
    return &_pair;
  }

  const JsonPair &operator*() const {
    return _pair;
  }

 private:
  JsonPair _pair;
};

// A read-write forward iterator for JsonArray
class JsonObjectIterator {
  typedef Internals::ListIterator<Internals::JsonPairData> internal_iterator;

 public:
  JsonObjectIterator() {}
  explicit JsonObjectIterator(Internals::MemoryPool *memoryPool,
                              internal_iterator iterator)
      : _memoryPool(memoryPool), _iterator(iterator) {}

  JsonPair operator*() const {
    return JsonPair(_memoryPool, &*_iterator);
  }
  JsonPairPtr operator->() {
    return JsonPairPtr(_memoryPool, &*_iterator);
  }

  bool operator==(const JsonObjectIterator &other) const {
    return _iterator == other._iterator;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _iterator != other._iterator;
  }

  JsonObjectIterator &operator++() {
    ++_iterator;
    return *this;
  }

  JsonObjectIterator &operator+=(size_t distance) {
    _iterator += distance;
    return *this;
  }

  internal_iterator internal() {
    return _iterator;
  }

 private:
  Internals::MemoryPool *_memoryPool;
  internal_iterator _iterator;
};
}  // namespace ArduinoJson
