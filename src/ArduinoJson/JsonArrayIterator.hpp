// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/SlotFunctions.hpp"
#include "JsonVariant.hpp"

namespace ARDUINOJSON_NAMESPACE {

class JsonVariantPtr {
 public:
  JsonVariantPtr(MemoryPool *memoryPool, JsonVariantData *data)
      : _variant(memoryPool, data) {}

  JsonVariant *operator->() {
    return &_variant;
  }

  JsonVariant &operator*() {
    return _variant;
  }

 private:
  JsonVariant _variant;
};

class JsonArrayIterator {
 public:
  JsonArrayIterator() : _slot(0) {}
  explicit JsonArrayIterator(MemoryPool *memoryPool, VariantSlot *slot)
      : _memoryPool(memoryPool), _slot(slot) {}

  JsonVariant operator*() const {
    return JsonVariant(_memoryPool, &_slot->value);
  }
  JsonVariantPtr operator->() {
    return JsonVariantPtr(_memoryPool, &_slot->value);
  }

  bool operator==(const JsonArrayIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _slot != other._slot;
  }

  JsonArrayIterator &operator++() {
    _slot = _slot->next;
    return *this;
  }

  JsonArrayIterator &operator+=(size_t distance) {
    _slot = slotAdvance(_slot, distance);
    return *this;
  }

  VariantSlot *internal() {
    return _slot;
  }

 private:
  MemoryPool *_memoryPool;
  VariantSlot *_slot;
};

class JsonVariantConstPtr {
 public:
  JsonVariantConstPtr(const JsonVariantData *data) : _variant(data) {}

  JsonVariantConst *operator->() {
    return &_variant;
  }

  JsonVariantConst &operator*() {
    return _variant;
  }

 private:
  JsonVariantConst _variant;
};

class JsonArrayConstIterator {
 public:
  JsonArrayConstIterator() : _slot(0) {}
  explicit JsonArrayConstIterator(const VariantSlot *slot) : _slot(slot) {}

  JsonVariantConst operator*() const {
    return JsonVariantConst(&_slot->value);
  }
  JsonVariantConstPtr operator->() {
    return JsonVariantConstPtr(&_slot->value);
  }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return _slot != other._slot;
  }

  JsonArrayConstIterator &operator++() {
    _slot = _slot->next;
    return *this;
  }

  JsonArrayConstIterator &operator+=(size_t distance) {
    _slot = slotAdvance(_slot, distance);
    return *this;
  }

  const VariantSlot *internal() {
    return _slot;
  }

 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
