// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "ForwardDeclarations.hpp"
#include "Internals/JsonNodeWrapper.hpp"

namespace ArduinoJson {

class JsonValue : public Internals::JsonNodeWrapper {
  friend class JsonArray;
  template <typename T>
  friend class JsonIterator;
  template <typename T>
  friend class JsonConstIterator;
  friend class JsonBuffer;
  friend class JsonObject;
  friend class JsonObjectKeyValue;

 public:
  JsonValue() {}

  void operator=(bool value);
  void operator=(const char *value);
  void operator=(double value) { set(value, 2); }
  void operator=(int value);
  void operator=(const JsonValue &value) { duplicate(value); }
  void operator=(const Internals::JsonNodeWrapper &object) {
    duplicate(object);
  }

  operator bool() const;
  operator const char *() const;
  operator double() const;
  operator long() const;
  operator int() const { return operator long(); }
  operator JsonArray() const;
  operator JsonObject() const;

  void set(double value, int decimals);

  template <typename T>
  T as() {
    return static_cast<T>(*this);
  }

 private:
  explicit JsonValue(Internals::JsonNode *node) : JsonNodeWrapper(node) {}
};
}
