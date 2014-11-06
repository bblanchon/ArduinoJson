// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {
class JsonArray;
class JsonObject;

class JsonBuffer {
 public:
  virtual ~JsonBuffer() {}

  JsonArray &createArray();
  JsonObject &createObject();

  JsonArray &parseArray(char *json, uint8_t nestingLimit = DEFAULT_LIMIT);
  JsonObject &parseObject(char *json, uint8_t nestingLimit = DEFAULT_LIMIT);

  virtual void *alloc(size_t size) = 0;

  static const uint8_t DEFAULT_LIMIT = 10;
};
}
