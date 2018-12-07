// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Variant/VariantRef.hpp"
#include "Key.hpp"

namespace ARDUINOJSON_NAMESPACE {
// A key value pair for CollectionData.
class Pair {
 public:
  Pair(MemoryPool* pool, VariantSlot* slot) : _key(slot) {
    if (slot) {
      _value = VariantRef(pool, slot->data());
    }
  }

  Key key() const {
    return _key;
  }

  VariantRef value() const {
    return _value;
  }

 private:
  Key _key;
  VariantRef _value;
};

class PairConst {
 public:
  PairConst(const VariantSlot* slot) : _key(slot) {
    if (slot) {
      _value = VariantConstRef(slot->data());
    }
  }

  Key key() const {
    return _key;
  }

  VariantConstRef value() const {
    return _value;
  }

 private:
  Key _key;
  VariantConstRef _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
