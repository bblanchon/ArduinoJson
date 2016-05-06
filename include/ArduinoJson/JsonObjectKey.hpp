// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "String.hpp"

namespace ArduinoJson {

// Represents a key in a JsonObject
class JsonObjectKey {
 public:
  JsonObjectKey(const char* key) : _value(key), _needs_copy(false) {}
  JsonObjectKey(const String& key) : _value(key.c_str()), _needs_copy(true) {}

  const char* c_str() const { return _value; }
  bool needs_copy() const { return _needs_copy; }

 private:
  const char* _value;
  bool _needs_copy;
};
}
