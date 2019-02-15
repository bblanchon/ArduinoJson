// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "../Strings/String.hpp"
#include "../Variant/VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {
// A key value pair for CollectionData.
class Pair {
 public:
  Pair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = String(slot->key(), !slot->ownsKey());
      _value = VariantRef(pool, slot->data());
    }
  }

  String key() const {
    return _key;
  }

  VariantRef value() const {
    return _value;
  }

 private:
  String _key;
  VariantRef _value;
};

class PairConst {
 public:
  PairConst(const VariantSlot* slot) {
    if (slot) {
      _key = String(slot->key(), !slot->ownsKey());
      _value = VariantConstRef(slot->data());
    }
  }

  String key() const {
    return _key;
  }

  VariantConstRef value() const {
    return _value;
  }

 private:
  String _key;
  VariantConstRef _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
