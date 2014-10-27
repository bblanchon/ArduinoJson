// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "JsonValueImpl.hpp"

namespace ArduinoJson {
namespace Internals {

struct JsonObjectNode {
  JsonObjectNode(const char* k) : key(k) {}

  const char* const key;
  JsonValueImpl value;
  JsonObjectNode* next;
};
}
}
