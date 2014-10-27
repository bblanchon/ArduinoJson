// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonContainer.hpp"
#include "Internals/JsonArrayImpl.hpp"

namespace ArduinoJson {
class JsonArray : public JsonContainer {
  friend class JsonValue;

 public:
  typedef JsonValue value_type;
  typedef Internals::JsonArrayIterator iterator;
  typedef Internals::JsonArrayConstIterator const_iterator;

  JsonArray() {}
  JsonArray(Internals::JsonArrayImpl* impl) : _impl(impl) {}

  value_type operator[](int index) const;

  value_type add();

  template <typename T>
  void add(T value) {
    add().set(value);
  }

  void add(double value, int decimals = 2) { add().set(value, decimals); }

  JsonArray createNestedArray();
  JsonObject createNestedObject();

  iterator begin() {
    if (!_impl) return end();
    return _impl->begin();
  }
  iterator end() { return iterator(0); }

  const_iterator begin() const {
    if (!_impl) return end();
    return const_cast<const Internals::JsonArrayImpl*>(_impl)->begin();
  }
  const_iterator end() const { return const_iterator(0); }

  static JsonArray null() { return JsonArray(NULL); }

 private:
  Internals::JsonArrayImpl* _impl;
};
}
