// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonPrintable.hpp"
#include "Internals/JsonObjectImpl.hpp"

namespace ArduinoJson {
class JsonObject : public JsonPrintable {
  friend class JsonValue;

 public:
  typedef const char* key_type;
  typedef JsonPair value_type;
  typedef Internals::JsonObjectIterator iterator;
  typedef Internals::JsonObjectConstIterator const_iterator;

  JsonObject() : _impl(NULL) {}
  JsonObject(Internals::JsonObjectImpl* impl) : _impl(impl) {}

  bool success() const { return _impl; }

  int size() const { return _impl ? _impl->size() : 0; }

  JsonValue operator[](key_type key);
  void remove(key_type key) {
    if (_impl) _impl->remove(key);
  }

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

  bool operator==(const JsonObject& other) const {
    return _impl == other._impl;
  }

 protected:
  virtual void writeTo(Internals::JsonWriter& writer) const {
    if (_impl) _impl->writeTo(writer);
  }

 private:
  Internals::JsonObjectImpl* _impl;
};
}
