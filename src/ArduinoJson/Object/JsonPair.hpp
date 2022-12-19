// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

namespace ARDUINOJSON_NAMESPACE {

// A key-value pair.
// https://arduinojson.org/v6/api/jsonobject/begin_end/
class JsonPair {
 public:
  // INTERNAL USE ONLY
  JsonPair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = JsonString(slot->key(), slot->ownsKey() ? JsonString::Copied
                                                     : JsonString::Linked);
      _value = JsonVariant(pool, slot->data());
    }
  }

  // Returns the key.
  JsonString key() const {
    return _key;
  }

  // Returns the value.
  JsonVariant value() const {
    return _value;
  }

 private:
  JsonString _key;
  JsonVariant _value;
};

// A read-only key-value pair.
// https://arduinojson.org/v6/api/jsonobjectconst/begin_end/
class JsonPairConst {
 public:
  JsonPairConst(const VariantSlot* slot) {
    if (slot) {
      _key = JsonString(slot->key(), slot->ownsKey() ? JsonString::Copied
                                                     : JsonString::Linked);
      _value = JsonVariantConst(slot->data());
    }
  }

  // Returns the key.
  JsonString key() const {
    return _key;
  }

  // Returns the value.
  JsonVariantConst value() const {
    return _value;
  }

 private:
  JsonString _key;
  JsonVariantConst _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
