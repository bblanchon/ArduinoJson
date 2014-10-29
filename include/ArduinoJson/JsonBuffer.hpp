// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <new>

#include "ForwardDeclarations.hpp"
#include "JsonValue.hpp"

namespace ArduinoJson {

class JsonBuffer {
 public:
  virtual ~JsonBuffer() {}

  JsonArray &createArray();
  JsonObject &createObject();

  JsonArray &parseArray(char *json);
  JsonObject &parseObject(char *json);

  virtual void *alloc(size_t size) = 0;
};
}
