// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonPair.hpp"

namespace ArduinoJson {

class JsonPairPtr {
 public:
  JsonPairPtr(Internals::MemoryPool *memoryPool, Internals::Slot *slot)
      : _pair(memoryPool, slot) {}

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

  explicit JsonObjectIterator(Internals::MemoryPool *memoryPool,
                              Internals::Slot *slot)
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

  Internals::Slot *internal() {
    return _slot;
  }

 private:
  Internals::MemoryPool *_memoryPool;
  Internals::Slot *_slot;
};
}  // namespace ArduinoJson
