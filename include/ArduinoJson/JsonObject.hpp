// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonContainer.hpp"
#include "Internals/JsonObjectImpl.hpp"

namespace ArduinoJson {
class JsonObject : public JsonContainer {
  friend class JsonValue;

 public:
  typedef const char* key_type;
  typedef JsonPair value_type;
  typedef Internals::JsonObjectIterator iterator;
  typedef Internals::JsonObjectConstIterator const_iterator;

  JsonObject(Internals::JsonObjectImpl* impl) : _impl(impl) {}

  JsonValue operator[](key_type key);
  void remove(key_type key);

  JsonArray createNestedArray(key_type key);
  JsonObject createNestedObject(key_type key);

  iterator begin() {
    if (!_impl) return end();
    return _impl->begin();
  }
  iterator end() { return iterator(0); }

  const_iterator begin() const {
    if (!_impl) return end();
    return const_cast<const Internals::JsonObjectImpl*>(_impl)->begin();
  }
  const_iterator end() const { return const_iterator(0); }

 private:
  Internals::JsonObjectImpl* _impl;
};
}
