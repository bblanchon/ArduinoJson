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

  template <typename T>
  void operator=(T value) {
    if (_impl) _impl->set(value);
  }

  void operator=(JsonArray array);
  void operator=(JsonObject object);

  void set(double value, int decimals) {
    if (_impl) _impl->set(value, decimals);
  }

  template <typename T>
  T as() {
    return static_cast<T>(*this);
  }

  operator bool() const { return _impl ? *_impl : false; }
  operator int() const { return _impl ? *_impl : 0; }
  operator long() const { return _impl ? *_impl : 0; }
  operator double() const { return _impl ? *_impl : 0.0; }
  operator const char *() const {
    return _impl ? *_impl : static_cast<char *>(NULL);
  }
  operator JsonArray() const;

  bool success() { return _impl; }

  static JsonValue null() { return JsonValue(NULL); }

 private:
  Internals::JsonValueImpl *_impl;
};
}
