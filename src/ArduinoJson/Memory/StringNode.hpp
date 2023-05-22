// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#include <stdint.h>  // uint16_t

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct StringNode {
  struct StringNode* next;
  uint16_t length;
  uint16_t references;
  char data[1];
};

// Returns the size (in bytes) of an string with n characters.
constexpr size_t sizeofString(size_t n) {
  return n + 1 + offsetof(StringNode, data);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
