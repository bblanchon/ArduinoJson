// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonKey.hpp"
#include "JsonVariant.hpp"

namespace ARDUINOJSON_NAMESPACE {
// A key value pair for JsonObjectData.
class JsonPair {
 public:
  JsonPair(MemoryPool* memoryPool, VariantSlot* slot) : _key(slot) {
    if (slot) {
      _value = JsonVariant(memoryPool, &slot->value);
    }
  }

  JsonKey key() const {
    return _key;
  }

  JsonVariant value() const {
    return _value;
  }

 private:
  JsonKey _key;
  JsonVariant _value;
};

class JsonPairConst {
 public:
  JsonPairConst(const VariantSlot* slot) : _key(slot) {
    if (slot) {
      _value = JsonVariantConst(&slot->value);
    }
  }

  JsonKey key() const {
    return _key;
  }

  JsonVariantConst value() const {
    return _value;
  }

 private:
  JsonKey _key;
  JsonVariantConst _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
