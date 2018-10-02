// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/Slot.hpp"
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
  explicit JsonArrayIterator(MemoryPool *memoryPool, Slot *iterator)
      : _memoryPool(memoryPool), _slot(iterator) {}

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
    while (distance && _slot) {
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
