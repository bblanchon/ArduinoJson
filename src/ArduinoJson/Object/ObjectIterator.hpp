// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Variant/SlotFunctions.hpp"
#include "Pair.hpp"

namespace ARDUINOJSON_NAMESPACE {

class PairPtr {
 public:
  PairPtr(MemoryPool *memoryPool, VariantSlot *slot)
      : _pair(memoryPool, slot) {}

  const Pair *operator->() const {
    return &_pair;
  }

  const Pair &operator*() const {
    return _pair;
  }

 private:
  Pair _pair;
};

class ObjectIterator {
 public:
  ObjectIterator() : _slot(0) {}

  explicit ObjectIterator(MemoryPool *memoryPool, VariantSlot *slot)
      : _memoryPool(memoryPool), _slot(slot) {}

  Pair operator*() const {
    return Pair(_memoryPool, _slot);
  }
  PairPtr operator->() {
    return PairPtr(_memoryPool, _slot);
  }

  bool operator==(const ObjectIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ObjectIterator &other) const {
    return _slot != other._slot;
  }

  ObjectIterator &operator++() {
    _slot = _slot->getNext();
    return *this;
  }

  ObjectIterator &operator+=(size_t distance) {
    _slot = _slot->getNext(distance);
    return *this;
  }

  VariantSlot *internal() {
    return _slot;
  }

 private:
  MemoryPool *_memoryPool;
  VariantSlot *_slot;
};

class PairConstPtr {
 public:
  PairConstPtr(const VariantSlot *slot) : _pair(slot) {}

  const PairConst *operator->() const {
    return &_pair;
  }

  const PairConst &operator*() const {
    return _pair;
  }

 private:
  PairConst _pair;
};

class ObjectConstIterator {
 public:
  ObjectConstIterator() : _slot(0) {}

  explicit ObjectConstIterator(const VariantSlot *slot) : _slot(slot) {}

  PairConst operator*() const {
    return PairConst(_slot);
  }
  PairConstPtr operator->() {
    return PairConstPtr(_slot);
  }

  bool operator==(const ObjectConstIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ObjectConstIterator &other) const {
    return _slot != other._slot;
  }

  ObjectConstIterator &operator++() {
    _slot = _slot->getNext();
    return *this;
  }

  ObjectConstIterator &operator+=(size_t distance) {
    _slot = _slot->getNext(distance);
    return *this;
  }

  const VariantSlot *internal() {
    return _slot;
  }

 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
