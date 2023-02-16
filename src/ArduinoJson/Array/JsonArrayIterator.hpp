// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/SlotFunctions.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class VariantPtr {
 public:
  VariantPtr(detail::MemoryPool* pool, detail::VariantData* data)
      : _variant(pool, data) {}

  JsonVariant* operator->() {
    return &_variant;
  }

  JsonVariant& operator*() {
    return _variant;
  }

 private:
  JsonVariant _variant;
};

class JsonArrayIterator {
  friend class JsonArray;

 public:
  JsonArrayIterator() : _slot(0) {}
  explicit JsonArrayIterator(detail::MemoryPool* pool,
                             detail::VariantSlot* slot)
      : _pool(pool), _slot(slot) {}

  JsonVariant operator*() const {
    return JsonVariant(_pool, _slot->data());
  }
  VariantPtr operator->() {
    return VariantPtr(_pool, _slot->data());
  }

  bool operator==(const JsonArrayIterator& other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonArrayIterator& other) const {
    return _slot != other._slot;
  }

  JsonArrayIterator& operator++() {
    _slot = _slot->next();
    return *this;
  }

  JsonArrayIterator& operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }

 private:
  detail::MemoryPool* _pool;
  detail::VariantSlot* _slot;
};

class VariantConstPtr {
 public:
  VariantConstPtr(const detail::VariantData* data) : _variant(data) {}

  JsonVariantConst* operator->() {
    return &_variant;
  }

  JsonVariantConst& operator*() {
    return _variant;
  }

 private:
  JsonVariantConst _variant;
};

class JsonArrayConstIterator {
  friend class JsonArray;

 public:
  JsonArrayConstIterator() : _slot(0) {}
  explicit JsonArrayConstIterator(const detail::VariantSlot* slot)
      : _slot(slot) {}

  JsonVariantConst operator*() const {
    return JsonVariantConst(_slot->data());
  }
  VariantConstPtr operator->() {
    return VariantConstPtr(_slot->data());
  }

  bool operator==(const JsonArrayConstIterator& other) const {
    return _slot == other._slot;
  }

  bool operator!=(const JsonArrayConstIterator& other) const {
    return _slot != other._slot;
  }

  JsonArrayConstIterator& operator++() {
    _slot = _slot->next();
    return *this;
  }

  JsonArrayConstIterator& operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }

 private:
  const detail::VariantSlot* _slot;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
