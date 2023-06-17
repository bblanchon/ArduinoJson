// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonPair.hpp>
#include <ArduinoJson/Variant/SlotFunctions.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonPairPtr {
 public:
  JsonPairPtr(detail::ResourceManager* resources, detail::VariantSlot* slot)
      : pair_(resources, slot) {}

  const JsonPair* operator->() const {
    return &pair_;
  }

  const JsonPair& operator*() const {
    return pair_;
  }

 private:
  JsonPair pair_;
};

class JsonObjectIterator {
  friend class JsonObject;

 public:
  JsonObjectIterator() : slot_(0) {}

  explicit JsonObjectIterator(detail::ResourceManager* resources,
                              detail::VariantSlot* slot)
      : resources_(resources), slot_(slot) {}

  JsonPair operator*() const {
    return JsonPair(resources_, slot_);
  }
  JsonPairPtr operator->() {
    return JsonPairPtr(resources_, slot_);
  }

  bool operator==(const JsonObjectIterator& other) const {
    return slot_ == other.slot_;
  }

  bool operator!=(const JsonObjectIterator& other) const {
    return slot_ != other.slot_;
  }

  JsonObjectIterator& operator++() {
    slot_ = slot_->next();
    return *this;
  }

  JsonObjectIterator& operator+=(size_t distance) {
    slot_ = slot_->next(distance);
    return *this;
  }

 private:
  detail::ResourceManager* resources_;
  detail::VariantSlot* slot_;
};

class JsonPairConstPtr {
 public:
  JsonPairConstPtr(const detail::VariantSlot* slot) : pair_(slot) {}

  const JsonPairConst* operator->() const {
    return &pair_;
  }

  const JsonPairConst& operator*() const {
    return pair_;
  }

 private:
  JsonPairConst pair_;
};

class JsonObjectConstIterator {
  friend class JsonObject;

 public:
  JsonObjectConstIterator() : slot_(0) {}

  explicit JsonObjectConstIterator(const detail::VariantSlot* slot)
      : slot_(slot) {}

  JsonPairConst operator*() const {
    return JsonPairConst(slot_);
  }
  JsonPairConstPtr operator->() {
    return JsonPairConstPtr(slot_);
  }

  bool operator==(const JsonObjectConstIterator& other) const {
    return slot_ == other.slot_;
  }

  bool operator!=(const JsonObjectConstIterator& other) const {
    return slot_ != other.slot_;
  }

  JsonObjectConstIterator& operator++() {
    slot_ = slot_->next();
    return *this;
  }

  JsonObjectConstIterator& operator+=(size_t distance) {
    slot_ = slot_->next(distance);
    return *this;
  }

 private:
  const detail::VariantSlot* slot_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
