// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>

#include "ForwardDeclarations.hpp"
#include "Internals/JsonValueImpl.hpp"

namespace ArduinoJson {

class JsonValue {
 public:
  JsonValue() : _impl(NULL) {}
  JsonValue(Internals::JsonValueImpl *impl) : _impl(impl) {}

  void set(const char *value) {
    if (_impl) _impl->set(value);
  }
  void set(bool value) {
    if (_impl) _impl->set(value);
  }
  void set(double value, int decimals = 2) {
    if (_impl) _impl->set(value, decimals);
  }
  void set(int value) {
    if (_impl) _impl->set(static_cast<long>(value));
  }
  void set(long value) {
    if (_impl) _impl->set(value);
  }
  void set(JsonObject object);
  void set(JsonArray array);

  operator bool() const { return _impl ? _impl->asBool() : false; }
  operator int() const { return _impl ? _impl->asLong() : 0; }
  operator long() const { return _impl ? _impl->asLong() : 0; }
  operator double() const { return _impl ? _impl->asDouble() : 0.0; }
  operator const char *() const {
    return _impl ? _impl->asString() : static_cast<char *>(NULL);
  }
  operator JsonArray();
  operator JsonObject();

  bool success() { return _impl; }

  template <typename T>
  JsonValue operator=(T value) {
    set(value);
    return *this;
  }

  template <typename T>
  T as() {
    return static_cast<T>(*this);
  }

 protected:
  virtual void writeTo(Internals::JsonWriter &writer) const {
    if (_impl) _impl->writeTo(writer);
  }

 private:
  Internals::JsonValueImpl *_impl;
};
}
