// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonPair.hpp>
#include <ArduinoJson/Variant/SlotFunctions.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonPairPtr {
 public:
  JsonPairPtr(MemoryPool* pool, VariantSlot* slot) : _pair(pool, slot) {}

  const JsonPair* operator->() const {
    return &_pair;
  }

  const JsonPair& operator*() const {
    return _pair;
  }

 private:
  JsonPair _pair;
};

class ObjectIterator {
  friend class JsonObject;

 public:
  ObjectIterator() : _slot(0) {}

  explicit ObjectIterator(MemoryPool* pool, VariantSlot* slot)
      : _pool(pool), _slot(slot) {}

  JsonPair operator*() const {
    return JsonPair(_pool, _slot);
  }
  JsonPairPtr operator->() {
    return JsonPairPtr(_pool, _slot);
  }

  bool operator==(const ObjectIterator& other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ObjectIterator& other) const {
    return _slot != other._slot;
  }

  ObjectIterator& operator++() {
    _slot = _slot->next();
    return *this;
  }

  ObjectIterator& operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }

 private:
  MemoryPool* _pool;
  VariantSlot* _slot;
};

class JsonPairConstPtr {
 public:
  JsonPairConstPtr(const VariantSlot* slot) : _pair(slot) {}

  const JsonPairConst* operator->() const {
    return &_pair;
  }

  const JsonPairConst& operator*() const {
    return _pair;
  }

 private:
  JsonPairConst _pair;
};

class ObjectConstIterator {
  friend class JsonObject;

 public:
  ObjectConstIterator() : _slot(0) {}

  explicit ObjectConstIterator(const VariantSlot* slot) : _slot(slot) {}

  JsonPairConst operator*() const {
    return JsonPairConst(_slot);
  }
  JsonPairConstPtr operator->() {
    return JsonPairConstPtr(_slot);
  }

  bool operator==(const ObjectConstIterator& other) const {
    return _slot == other._slot;
  }

  bool operator!=(const ObjectConstIterator& other) const {
    return _slot != other._slot;
  }

  ObjectConstIterator& operator++() {
    _slot = _slot->next();
    return *this;
  }

  ObjectConstIterator& operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }

 private:
  const VariantSlot* _slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
