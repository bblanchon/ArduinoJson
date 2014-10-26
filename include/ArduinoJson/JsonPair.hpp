// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonValue.hpp"

namespace ArduinoJson {
class JsonPair {
 public:
  JsonPair(const char *k) : _key(k) {}

  const char *key() const { return _key; }
  JsonValue &value() { return _value; }

 private:
  const char *_key;
  JsonValue _value;
};
}
