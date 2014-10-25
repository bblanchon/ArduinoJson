// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonPairInternal.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonObjectConstIterator {
 public:
  explicit JsonObjectConstIterator(Internals::JsonNode *node) : _pair(node) {}

  JsonPair operator*() const { return _pair; }
  JsonPair *operator->() { return &_pair; }

  bool operator==(const JsonObjectConstIterator &other) const {
    return _pair.isSameAs(other._pair);
  }

  bool operator!=(const JsonObjectConstIterator &other) const {
    return !_pair.isSameAs(other._pair);
  }

  JsonObjectConstIterator &operator++() {
    _pair.moveToNext();
    return *this;
  }

 private:
  JsonPairInternal _pair;
};
}
}
