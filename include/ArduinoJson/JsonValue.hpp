// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>

#include "ForwardDeclarations.hpp"
#include "Internals/JsonValueContent.hpp"
#include "Internals/JsonValueType.hpp"

namespace ArduinoJson {

class JsonValue {
 public:
  JsonValue() : _type(Internals::JSON_UNDEFINED) {}

  void set(bool value);
  void set(const char *value);
  void set(double value, int decimals = 2);
  void set(int value) { set(static_cast<long>(value)); }
  void set(long value);
  void set(JsonArray &array);
  void set(JsonObject &object);

  JsonArray &asArray();
  JsonObject &asObject();
  bool asBool() const;
  const char *asString() const;
  double asDouble() const;
  long asLong() const;

  template <typename T>
  JsonValue &operator=(T value) {
    set(value);
    return *this;
  }

  static JsonValue &invalid() { return _invalid; }

  bool success() { return _type != Internals::JSON_INVALID; }

  void writeTo(Internals::JsonWriter &writer) const;

 private:
  Internals::JsonValueType _type;
  Internals::JsonValueContent _content;
  static JsonValue _invalid;
};
}
