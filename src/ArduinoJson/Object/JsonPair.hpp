// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Variant/VariantConstRef.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {
// A key value pair for CollectionData.
class JsonPair {
 public:
  JsonPair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = JsonString(slot->key(), slot->ownsKey() ? JsonString::Copied
                                                     : JsonString::Linked);
      _value = VariantRef(pool, slot->data());
    }
  }

  JsonString key() const {
    return _key;
  }

  VariantRef value() const {
    return _value;
  }

 private:
  JsonString _key;
  VariantRef _value;
};

class JsonPairConst {
 public:
  JsonPairConst(const VariantSlot* slot) {
    if (slot) {
      _key = JsonString(slot->key(), slot->ownsKey() ? JsonString::Copied
                                                     : JsonString::Linked);
      _value = VariantConstRef(slot->data());
    }
  }

  JsonString key() const {
    return _key;
  }

  VariantConstRef value() const {
    return _value;
  }

 private:
  JsonString _key;
  VariantConstRef _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
