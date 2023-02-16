// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A key-value pair.
// https://arduinojson.org/v6/api/jsonobject/begin_end/
class JsonPair {
 public:
  // INTERNAL USE ONLY
  JsonPair(detail::MemoryPool* pool, detail::VariantSlot* slot) {
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
  JsonPairConst(const detail::VariantSlot* slot) {
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

ARDUINOJSON_END_PUBLIC_NAMESPACE
