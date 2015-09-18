// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Arduino/String.hpp"

namespace ArduinoJson {

class JsonObjectKey {
 public:
  JsonObjectKey() {}
  JsonObjectKey(const char* key) : _data(key) {}
  JsonObjectKey(const String& key) : _data(key.c_str()) {}

  operator const char*() const { return _data; }

 private:
  const char* _data;
};
}
