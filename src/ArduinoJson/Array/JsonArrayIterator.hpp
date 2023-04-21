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
      : variant_(pool, data) {}

  JsonVariant* operator->() {
    return &variant_;
  }

  JsonVariant& operator*() {
    return variant_;
  }

 private:
  JsonVariant variant_;
};

class JsonArrayIterator {
  friend class JsonArray;

 public:
  JsonArrayIterator() : slot_(0) {}
  explicit JsonArrayIterator(detail::MemoryPool* pool,
                             detail::VariantSlot* slot)
      : pool_(pool), slot_(slot) {}

  JsonVariant operator*() const {
    return JsonVariant(pool_, slot_->data());
  }
  VariantPtr operator->() {
    return VariantPtr(pool_, slot_->data());
  }

  bool operator==(const JsonArrayIterator& other) const {
    return slot_ == other.slot_;
  }

  bool operator!=(const JsonArrayIterator& other) const {
    return slot_ != other.slot_;
  }

  JsonArrayIterator& operator++() {
    slot_ = slot_->next();
    return *this;
  }

  JsonArrayIterator& operator+=(size_t distance) {
    slot_ = slot_->next(distance);
    return *this;
  }

 private:
  detail::MemoryPool* pool_;
  detail::VariantSlot* slot_;
};

class VariantConstPtr {
 public:
  VariantConstPtr(const detail::VariantData* data) : variant_(data) {}

  JsonVariantConst* operator->() {
    return &variant_;
  }

  JsonVariantConst& operator*() {
    return variant_;
  }

 private:
  JsonVariantConst variant_;
};

class JsonArrayConstIterator {
  friend class JsonArray;

 public:
  JsonArrayConstIterator() : slot_(0) {}
  explicit JsonArrayConstIterator(const detail::VariantSlot* slot)
      : slot_(slot) {}

  JsonVariantConst operator*() const {
    return JsonVariantConst(slot_->data());
  }
  VariantConstPtr operator->() {
    return VariantConstPtr(slot_->data());
  }

  bool operator==(const JsonArrayConstIterator& other) const {
    return slot_ == other.slot_;
  }

  bool operator!=(const JsonArrayConstIterator& other) const {
    return slot_ != other.slot_;
  }

  JsonArrayConstIterator& operator++() {
    slot_ = slot_->next();
    return *this;
  }

  JsonArrayConstIterator& operator+=(size_t distance) {
    slot_ = slot_->next(distance);
    return *this;
  }

 private:
  const detail::VariantSlot* slot_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
