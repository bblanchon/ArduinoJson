// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/SlotFunctions.hpp"
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
    _slot = slotAdvance(_slot, distance);
    return *this;
  }

  Slot *internal() {
    return _slot;
  }

 private:
  MemoryPool *_memoryPool;
  Slot *_slot;
};

class JsonPairConstPtr {
 public:
  JsonPairConstPtr(const Slot *slot) : _pair(slot) {}

  const JsonPairConst *operator->() const {
    return &_pair;
  }

  const JsonPairConst &operator*() const {
    return _pair;
  }

 private:
  JsonPairConst _pair;
};

class JsonObjectConstIterator {
 public:
  JsonObjectConstIterator() : _slot(0) {}

  explicit JsonObjectConstIterator(const Slot *slot) : _slot(slot) {}

  JsonPairConst operator*() const {
    return JsonPairConst(_slot);
  }
  JsonPairConstPtr operator->() {
    return JsonPairConstPtr(_slot);
  }

  bool operator==(const JsonObjectConstIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonObjectConstIterator &other) const {
    return _slot != other._slot;
  }

  JsonObjectConstIterator &operator++() {
    if (_slot) _slot = _slot->next;
    return *this;
  }

  JsonObjectConstIterator &operator+=(size_t distance) {
    _slot = slotAdvance(_slot, distance);
    return *this;
  }

  const Slot *internal() {
    return _slot;
  }

 private:
  const Slot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
