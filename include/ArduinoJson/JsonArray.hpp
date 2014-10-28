// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonPrintable.hpp"
#include "Internals/JsonArrayImpl.hpp"

namespace ArduinoJson {
class JsonArray : public JsonPrintable {
  friend class JsonValue;

 public:
  typedef JsonValue value_type;
  typedef Internals::JsonArrayIterator iterator;
  typedef Internals::JsonArrayConstIterator const_iterator;

  JsonArray() : _impl(NULL) {}
  JsonArray(Internals::JsonArrayImpl* impl) : _impl(impl) {}

  bool success() const { return _impl; }

  int size() const { return _impl ? _impl->size() : 0; }

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

  bool operator==(const JsonArray& other) const { return _impl == other._impl; }

 protected:
  virtual void writeTo(Internals::JsonWriter& writer) const {
    if (_impl) _impl->writeTo(writer);
  }

 private:
  Internals::JsonArrayImpl* _impl;
};
}
