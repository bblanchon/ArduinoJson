// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Variant/SlotFunctions.hpp"
#include "../Variant/VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

class VariantPtr {
 public:
  VariantPtr(MemoryPool *memoryPool, VariantData *data)
      : _variant(memoryPool, data) {}

  VariantRef *operator->() {
    return &_variant;
  }

  VariantRef &operator*() {
    return _variant;
  }

 private:
  VariantRef _variant;
};

class ArrayIterator {
 public:
  ArrayIterator() : _slot(0) {}
  explicit ArrayIterator(MemoryPool *memoryPool, VariantSlot *slot)
      : _memoryPool(memoryPool), _slot(slot) {}

  VariantRef operator*() const {
    return VariantRef(_memoryPool, _slot->getData());
  }
  VariantPtr operator->() {
    return VariantPtr(_memoryPool, _slot->getData());
  }

  bool operator==(const ArrayIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ArrayIterator &other) const {
    return _slot != other._slot;
  }

  ArrayIterator &operator++() {
    _slot = _slot->getNext();
    return *this;
  }

  ArrayIterator &operator+=(size_t distance) {
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

class VariantConstPtr {
 public:
  VariantConstPtr(const VariantData *data) : _variant(data) {}

  VariantConstRef *operator->() {
    return &_variant;
  }

  VariantConstRef &operator*() {
    return _variant;
  }

 private:
  VariantConstRef _variant;
};

class ArrayConstRefIterator {
 public:
  ArrayConstRefIterator() : _slot(0) {}
  explicit ArrayConstRefIterator(const VariantSlot *slot) : _slot(slot) {}

  VariantConstRef operator*() const {
    return VariantConstRef(_slot->getData());
  }
  VariantConstPtr operator->() {
    return VariantConstPtr(_slot->getData());
  }

  bool operator==(const ArrayConstRefIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ArrayConstRefIterator &other) const {
    return _slot != other._slot;
  }

  ArrayConstRefIterator &operator++() {
    _slot = _slot->getNext();
    return *this;
  }

  ArrayConstRefIterator &operator+=(size_t distance) {
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
