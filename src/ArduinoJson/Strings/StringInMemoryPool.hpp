// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string.h>
#include "../Memory/StringSlot.hpp"

namespace ARDUINOJSON_NAMESPACE {

class StringInMemoryPool {
 public:
  StringInMemoryPool(StringSlot* s = 0) : _slot(s) {}

  bool equals(const char* expected) const {
    if (!_slot) return expected == 0;
    const char* actual = _slot->value;
    if (actual == expected) return true;
    return strcmp(actual, expected) == 0;
  }

  bool isNull() const {
    return !_slot;
  }

  template <typename TMemoryPool>
  StringSlot* save(TMemoryPool*) const {
    return _slot;
  }

  size_t size() const {
    return _slot->size;
  }

  StringSlot* slot() const {
    return _slot;
  }

  const char* c_str() const {
    return _slot->value;
  }

 protected:
  StringSlot* _slot;
};

}  // namespace ARDUINOJSON_NAMESPACE
