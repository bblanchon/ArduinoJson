// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonPair.hpp>
#include <ArduinoJson/Variant/SlotFunctions.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

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
  Ptr<JsonPair> operator->() {
    return operator*();
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

class JsonObjectConstIterator {
  friend class JsonObject;

 public:
  JsonObjectConstIterator() : slot_(0) {}

  explicit JsonObjectConstIterator(const detail::VariantSlot* slot)
      : slot_(slot) {}

  JsonPairConst operator*() const {
    return JsonPairConst(slot_);
  }
  Ptr<JsonPairConst> operator->() {
    return operator*();
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
