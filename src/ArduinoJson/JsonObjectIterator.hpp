// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonPair.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonPairPtr {
 public:
  JsonPairPtr(MemoryPool *memoryPool, Slot *slot) : _pair(memoryPool, slot) {}

  const JsonPair *operator->() const {
    return &_pair;
  }

  const JsonPair &operator*() const {
    return _pair;
  }

 private:
  JsonPair _pair;
};

class JsonObjectIterator {
 public:
  JsonObjectIterator() : _slot(0) {}

  explicit JsonObjectIterator(MemoryPool *memoryPool, Slot *slot)
      : _memoryPool(memoryPool), _slot(slot) {}

  JsonPair operator*() const {
    return JsonPair(_memoryPool, _slot);
  }
  JsonPairPtr operator->() {
    return JsonPairPtr(_memoryPool, _slot);
  }

  bool operator==(const JsonObjectIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _slot != other._slot;
  }

  JsonObjectIterator &operator++() {
    if (_slot) _slot = _slot->next;
    return *this;
  }

  JsonObjectIterator &operator+=(size_t distance) {
    while (_slot && distance > 0) {
      _slot = _slot->next;
      distance--;
    }
    return *this;
  }

  Slot *internal() {
    return _slot;
  }

 private:
  MemoryPool *_memoryPool;
  Slot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
